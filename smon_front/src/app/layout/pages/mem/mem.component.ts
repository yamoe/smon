import { Component, ViewChild, OnInit, OnDestroy } from '@angular/core';
import { DataService, ChartConfig, Util } from '../../../common';
import { Subscription } from 'rxjs/Subscription';


@Component({
  selector: 'app-mem',
  template: `
  <div class="row">
    <div class="col-md-12">
      <app-chart [config]="config"></app-chart>
    </div>
  </div>  
  `
})
export class MemComponent implements OnInit, OnDestroy {
  public config: ChartConfig;
  private websocketSubscription: Subscription;

  constructor(private dataService: DataService) { }

  ngOnInit() {
    this.config = ChartConfig.newByteType('total');
    this.websocketSubscription = this.dataService.resMem().subscribe(data => {

      let date = Util.toLocalDate(data.utc);
      
      this.config.addWithBytePercentageDesc(date, data.use, data.total);
      if (!this.config.yScaleMax) {
        this.config.yScaleMax = data.total;
      }
    });
  }

  ngOnDestroy() {
    if (this.websocketSubscription) {
      this.websocketSubscription.unsubscribe();
      this.websocketSubscription = null;
    }
  }

}
