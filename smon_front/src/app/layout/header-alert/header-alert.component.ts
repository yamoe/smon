import { Component, OnInit, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs/Subscription';

import { DataService } from '../../common';

@Component({
  selector: 'app-header-alert',
  templateUrl: './header-alert.component.html'
})
export class HeaderAlertComponent implements OnInit, OnDestroy {

  public show: boolean = false;
  private disconnectedSubscription: Subscription;
  
  constructor(private dataService: DataService) {
    this.disconnectedSubscription = dataService.onIsDisconnected().subscribe(() =>{
      this.show = true;
    });
  }

  ngOnInit() { }

  ngOnDestroy() {
    this.disconnectedSubscription.unsubscribe();
  }

  public close(): void {
    this.show = false;
  }

  public refresh(): void {
    //history.go(0);
    window.location.reload(true);
  }
}
