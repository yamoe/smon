import { Injectable, OnInit, OnDestroy } from '@angular/core';
import 'rxjs/add/operator/toPromise';

import { Observable } from 'rxjs/Rx';
import { Subject } from 'rxjs/Subject';
import { Subscription } from 'rxjs/Subscription';
import { StompConfig, StompService, StompState } from '@stomp/ng2-stompjs';
import * as SockJS from 'sockjs-client';

import { WebsocketMessenger } from './websocket-messenger';
import { CpuData, DiskData, MemData, NetData, SysData, ProcData } from '../';

import { environment } from '../../../environments/environment'

/**
 * 서버와 websocket, restful 통신으로 데이타 제공
 */

@Injectable()
export class DataService implements OnInit, OnDestroy {

  private stomp: StompService;
  private isDisconnected: Subject<void> = new Subject<void>();
  
  private cpu: WebsocketMessenger<CpuData>;
  private disk: WebsocketMessenger<DiskData>;
  private mem: WebsocketMessenger<MemData>;
  private net: WebsocketMessenger<NetData>;
  private sys: WebsocketMessenger<SysData>;
  private proc: WebsocketMessenger<ProcData>;
  private procList: WebsocketMessenger<string[]>;
  private procRegister: WebsocketMessenger<{}>;
  private procUnregister: WebsocketMessenger<{}>;


  constructor() {


    this.stomp = new StompService({
      url: new SockJS(environment.url.websocket.base.server),
      headers: {},
      heartbeat_in: 0,
      heartbeat_out: 20000,
      reconnect_delay: 5000,
      debug: false
    });


    // reconnect가 정상동작 하지 않음 : https://github.com/stomp-js/ng2-stompjs/issues/35
    this.stomp.state
      .subscribe((state: number) => {
        if (state == StompState.CLOSED) {
          this.isDisconnected.next();
        }
      });

    let baseSub = environment.url.websocket.base.subscribe;
    let basePub = environment.url.websocket.base.publish;
    let path = environment.url.websocket.path;

    this.cpu = new WebsocketMessenger<CpuData>(this.stomp, baseSub + path.cpu, basePub + path.cpu);
    this.disk = new WebsocketMessenger<DiskData>(this.stomp, baseSub + path.disk, basePub + path.disk);
    this.mem = new WebsocketMessenger<MemData>(this.stomp, baseSub + path.mem, basePub + path.mem);
    this.net = new WebsocketMessenger<NetData>(this.stomp, baseSub + path.net, basePub + path.net);
    this.sys = new WebsocketMessenger<SysData>(this.stomp, baseSub + path.sys, basePub + path.sys);
    this.proc = new WebsocketMessenger<ProcData>(this.stomp, baseSub + path.proc, basePub + path.proc);
    this.procList = new WebsocketMessenger<string[]>(this.stomp, baseSub + path.procList, basePub + path.procList);
    this.procRegister = new WebsocketMessenger(this.stomp, baseSub + path.procRegist, basePub + path.procRegist);
    this.procUnregister = new WebsocketMessenger(this.stomp, baseSub + path.procUnregist, basePub + path.procUnregist);

  }

  public onIsDisconnected(): Observable<void> {
    return this.isDisconnected.asObservable();
  }

  ngOnInit() {
    console.log("DataService ngOnInit");
  }

  ngOnDestroy() {
    this.cpu.stop();
    this.disk.stop();
    this.mem.stop();
    this.net.stop();
    this.proc.stop();
    this.sys.stop();
    this.procList.stop();
    this.procRegister.stop();
    this.procUnregister.stop();
  }

  public resCpu(): Observable<CpuData> {
    return this.cpu.subscribe();
  }

  public resDisk(): Observable<DiskData> {
    return this.disk.subscribe();
  }

  public resMem(): Observable<MemData> {
    return this.mem.subscribe();
  }

  public resNet(): Observable<NetData> {
    return this.net.subscribe();
  }

  public resProc(): Observable<ProcData> {
    return this.proc.subscribe();
  }

  public resSys(): Observable<SysData> {
    return this.sys.subscribe();
  }

  public reqSys(): void {
    this.sys.publish();
  }

  public resProcessList(): Observable<string[]> {
    return this.procList.subscribe();
  }

  public reqProcessList(): void {
    this.procList.publish();
  }

  public registProc(name: string): void {
    this.procRegister.publish(name);
  }

  public unregistProc(name: string): void {
    this.procUnregister.publish(name);
  }

}

