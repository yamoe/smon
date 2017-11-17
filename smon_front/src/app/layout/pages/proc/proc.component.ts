import { Component, ViewChild, OnInit, OnDestroy } from '@angular/core';
import { DataService, ChartConfig, ChartComponent, DynamicComponent, DiskData, Util } from '../../../common';
import { Subscription } from 'rxjs/Subscription';

const Names = {
  CPU: 'cpu',
  PMEM: 'pmem',
  VMEM: 'vmem',
  IO_TOTAL: 'io total',
  IO_READ: 'io read',
  IO_WRITE: 'io write',
  TH_CNT: 'thread count',
  TCP_CNT: 'tcp count',
  TCP_LISTEN_CNT: 'tcp listen count',
  TCP_ESTAB_CNT: ' tcp estab count',
  UDP_CNT: 'udp count'
};

@Component({
  selector: 'app-proc',
  templateUrl: './proc.component.html'
})
export class ProcComponent implements OnInit {
 
  public loadingProcessList: boolean = false; // process list 로딩 중 플래그
  public processList: string[] = [];  // 조회한 process list 목록
  private processListSubscription: Subscription;

  // 생성된 Chart 들을 제어하기 위한 ChartConfig 맵(json)
  // key1 : process name, key2 : pid, key3 : Names, value : CharConfig>>
  private configs: { [key: string]: { [key: number]: { [key: string]: ChartConfig } } } = {};
  private websocketSubscription: Subscription;
  
  @ViewChild('dynamic') private dynamic: DynamicComponent;

  constructor(private dataService: DataService) { }

  ngOnInit() {
    this.processListSubscription = this.dataService.resProcessList().subscribe(data => {
      this.processList = data;
      this.loadingProcessList = false;
    });

    this.loadProcessList();

    this.websocketSubscription = this.dataService.resProc().subscribe(data => {

      let date = Util.toLocalDate(data.utc);
      let keys = Object.keys(data.proc);
      
      for (let deletedKey of this.dynamic.removeIfNotExist(keys)) {
        delete this.configs[deletedKey];
      }

      keys.forEach(function (processName) {

        if (!this.dynamic.exist(processName)) {
          this.addProcessComponent(processName);
          return;
        }

        if (!this.configs[processName]) return;

        let pids: string[] = [];
        for (let usage of data.proc[processName]) {
          let pid = usage.pid.toString();
          pids.push(pid);

          let chartConfigs = this.configs[processName][pid];

          if (!chartConfigs) {
            this.addPidComponent(processName, pid);
          } else {
            // add data
            chartConfigs[Names.CPU].addWithPercentageDesc(date, usage.cpu);
            chartConfigs[Names.PMEM].addWithByteDesc(date, usage.pmem);
            chartConfigs[Names.VMEM].addWithByteDesc(date, usage.vmem);
            chartConfigs[Names.IO_TOTAL].addWithByteDesc(date, usage.ioTotal);
            chartConfigs[Names.IO_READ].addWithByteDesc(date, usage.ioRead);
            chartConfigs[Names.IO_WRITE].addWithByteDesc(date, usage.ioWrite);
            chartConfigs[Names.TH_CNT].addWith(date, usage.thCnt);
            chartConfigs[Names.TCP_CNT].addWith(date, usage.tcpCnt);
            chartConfigs[Names.TCP_LISTEN_CNT].addWith(date, usage.tcpListenCnt);
            chartConfigs[Names.TCP_ESTAB_CNT].addWith(date, usage.tcpEstabCnt);
            chartConfigs[Names.UDP_CNT].addWith(date, usage.udpCnt);
          }

        }
        for (let deletedKey of this.dynamic.get(processName).removeIfNotExist(pids)) {
          delete this.configs[processName][deletedKey];
        }
  
      }, this);


    });
  }


  ngOnDestroy() {
    if (this.websocketSubscription) {
      this.websocketSubscription.unsubscribe();
      this.websocketSubscription = null;
    }

    if (this.processListSubscription) {
      this.processListSubscription.unsubscribe();
      this.processListSubscription = null;
    }
  }


  private addProcessComponent(processName: string): void {
    this.dynamic.add(
      processName,
      DynamicComponent,
      {
        key: processName,
        cols: 1,
        columnStyle: 'col-md-12',
        titleFormatting: () => '<h3>' + processName +'</h3>',
        showTitleHRTag: true,
        showCloseButton: true,
        bottomMargin: 2
      },
      {
        onAfterViewInit: this.onAfterViewInitProcess.bind(this),
        onClose: this.onCloseProcess.bind(this)
      }
    );
  }

  /** 프로세스용 컴포넌트 등록 완료 이벤트 수신 */
  private onAfterViewInitProcess(processName: string): void {
    // marking
    this.configs[processName] = {};
  }

  /** 삭제 버튼 이벤트 발생 시 제거 */
  private onCloseProcess(processName: string): void {
    this.dataService.unregistProc(processName);
    this.dynamic.remove(processName);
    delete this.configs[processName];
  }

  private addPidComponent(processName:string, pid: string): void {
    let cmp: DynamicComponent = this.dynamic.get(processName);

    cmp.add(
      pid,
      DynamicComponent,
      {
        key: pid,
        cols: 4,
        columnStyle: 'col-xs-12 col-sm-6 col-md-6 col-lg-3 col-xl-3',
        titleFormatting: () => 'pid : ' + pid +'',
        showTitleHRTag: true,
        showCloseButton: false,
        topMargin: 1,
        bottomMargin: 1
      },
      {
        onAfterViewInit: this.onAfterViewInitPid.bind(this, processName),
        onClose: this.onClosePid.bind(this, processName)
      }
    );
  }

  /** process id (pid) 용 컴포넌트 등록 완료시 chart componenet 추가 */
  private onAfterViewInitPid(processName: string, pid: string): void {
    // add chart component
    let chartConfigs = this.configs[processName][pid];
    if (!chartConfigs) {
      chartConfigs = this.configs[processName][pid] = {};
    }
    
    let cmp: DynamicComponent = this.dynamic.get(processName).get(pid);

    this.addChartComponent(chartConfigs, cmp, Names.CPU, ChartConfig.newCpuType);
    this.addChartComponent(chartConfigs, cmp, Names.PMEM, ChartConfig.newByteType);
    this.addChartComponent(chartConfigs, cmp, Names.VMEM, ChartConfig.newByteType);
    this.addChartComponent(chartConfigs, cmp, Names.IO_TOTAL, ChartConfig.newByteType);
    this.addChartComponent(chartConfigs, cmp, Names.IO_READ, ChartConfig.newByteType);
    this.addChartComponent(chartConfigs, cmp, Names.IO_WRITE, ChartConfig.newByteType);
    this.addChartComponent(chartConfigs, cmp, Names.TH_CNT, ChartConfig.newCountType);
    this.addChartComponent(chartConfigs, cmp, Names.TCP_CNT, ChartConfig.newCountType);
    this.addChartComponent(chartConfigs, cmp, Names.TCP_LISTEN_CNT, ChartConfig.newCountType);
    this.addChartComponent(chartConfigs, cmp, Names.TCP_ESTAB_CNT, ChartConfig.newCountType);
    this.addChartComponent(chartConfigs, cmp, Names.UDP_CNT, ChartConfig.newCountType);

  }

  /** nothing to do */
  private onClosePid(processName: string, pid: string): void {
    /*
    let cmp: DynamicComponent = this.dynamic.get(processName);
    cmp.remove(pid);
    delete this.configs[processName][pid];
    */
  }

  private addChartComponent(configs:any, cmp: DynamicComponent, chartName: string, newChartConfigFunction: Function): void {
    let config = newChartConfigFunction(chartName);
    configs[chartName] = config;
    cmp.add(
      chartName,
      ChartComponent, 
      {
       'config': config
      }
    );
  }

  /** process list 조회 */
  public loadProcessList() {
    this.loadingProcessList = true;
    this.dataService.reqProcessList();
  }

  /** 모니터링할 process 등록 */
  public registProcessName(processName: string): void {
    if (processName.length <= 0) return;
    this.dataService.registProc(processName);
  }

}
