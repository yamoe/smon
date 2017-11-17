import { Component, OnInit, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs/Subscription';

import { DataService, SysData, Util } from '../../common';

@Component({
  selector: 'app-header',
  templateUrl: './header.component.html'
})
export class HeaderComponent implements OnInit, OnDestroy {

  public data: SysData;
  private subscription: Subscription;

  constructor(private dataService: DataService) { }

  ngOnInit() {
    this.subscription = this.dataService.resSys().subscribe(data => {
      this.data = data;
    });
    this.query();
  }

  ngOnDestroy() {
    if (this.subscription) {
      this.subscription.unsubscribe();
      this.subscription = null;
    }
  }

  public query(): void {
    this.dataService.reqSys();
  }

}
