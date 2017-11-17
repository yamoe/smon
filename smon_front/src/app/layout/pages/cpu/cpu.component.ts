import { Component, ViewContainerRef, ViewChild, OnInit, OnDestroy } from '@angular/core';
import { DataService, ChartConfig, DynamicComponent, ChartComponent, CpuData, Util } from '../../../common';
import { Subscription } from 'rxjs/Subscription';


@Component({
  selector: 'app-cpu',
  template: `
  <div class="row">
    <div class="col-md-12">
      <app-chart [config]="totalConfig"></app-chart>
    </div>
  </div>
  <div class="row">
    <div class="col-md-12">
      <app-dynamic #dynamic [cols]="cols" [columnStyle]="columnStyle" [titleFormatting]="titleFormatting" [showTitleHRTag]="true"></app-dynamic>
    </div>
  </div>
  `
})

export class CpuComponent implements OnInit, OnDestroy {

  public cols = 4;
  public columnStyle = 'col-xs-12 col-sm-6 col-md-6 col-lg-6 col-xl-3';
  public titleFormatting = () => '<br/><h3>cores</h3>';

  public totalConfig: ChartConfig = ChartConfig.newCpuType('total');
  private coreConfigs: { [key: string]: ChartConfig } = {};

  private websocketSubscription: Subscription;
  @ViewChild('dynamic') private dynamic: DynamicComponent;

  constructor(private dataService: DataService) {}

  ngOnInit() {
    this.websocketSubscription = this.dataService.resCpu().subscribe(data => {

      let date = Util.toLocalDate(data.utc);

      // total
      this.totalConfig.addWithPercentageDesc(date, data.total);

      // cores
      for (let i = 0; i < data.cores.length; ++i) {
        let name = i.toString();

        if (!this.dynamic.exist(name)) {
          this.addChartComponent(name);
          continue;
        }

        // add data
        this.coreConfigs[name].addWithPercentageDesc(date, data.cores[i]);
      }
    });

  }

  ngOnDestroy() {
    if (this.websocketSubscription) {
      this.websocketSubscription.unsubscribe();
      this.websocketSubscription = null;
    }
  }

  private addChartComponent(chartName: string): void {

    let config = ChartConfig.newCpuType('core' + chartName);
    this.coreConfigs[chartName] = config;

    this.dynamic.add(
      chartName,
      ChartComponent, 
      {
       'config': config
      }
    );
  }

}
