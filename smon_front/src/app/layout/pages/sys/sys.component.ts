import { Component, OnInit, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs/Subscription';

import { DataService, SysData, Util } from '../../../common';

@Component({
  selector: 'app-sys',
  templateUrl: './sys.component.html'
})
export class SysComponent implements OnInit, OnDestroy {

  public data: SysData;
  private websocketSubscription: Subscription;

  constructor(private dataService: DataService) { }

  ngOnInit() {
    this.websocketSubscription = this.dataService.resSys().subscribe(data => {
      this.data = data;
    });
    this.query();
  }

  ngOnDestroy() {
    if (this.websocketSubscription) {
      this.websocketSubscription.unsubscribe();
      this.websocketSubscription = null;
    }
  }

  private queryTime() : string {
    return Util.toDateString(Util.toLocalDate(this.data.utc));
  }

  public query(): void {
    this.dataService.reqSys();
  }


}
