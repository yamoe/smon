import { Component, ViewChild, OnInit, OnDestroy } from '@angular/core';
import { DataService, ChartConfig, ChartComponent, DynamicComponent, DiskData, Util } from '../../../common';
import { Subscription } from 'rxjs/Subscription';

const Names = {
  TOTAL: 'total',
  SENT: 'sent',
  RECVED: 'recved'
};

@Component({
  selector: 'app-net',
  template: `
    <div class="row">
      <div class="col-md-12">
        <app-dynamic #dynamic></app-dynamic>
      </div>
    </div>
  `
})
export class NetComponent implements OnInit, OnDestroy {
  // key1 : disk name, key2 : Names, value : CharConfig>>
  private configs: { [key: string]: { [key: string]: ChartConfig } } = {};
  private websocketSubscription: Subscription;
  @ViewChild('dynamic') private dynamic: DynamicComponent;

  constructor(private dataService: DataService) { }

  ngOnInit() {
    this.websocketSubscription = this.dataService.resNet().subscribe(data => {
      
      let keys = Object.keys(data.net);

      for (let deletedKey of this.dynamic.removeIfNotExist(keys)) {
        delete this.configs[deletedKey];
      }

      keys.forEach(function (name) {
        if (!this.dynamic.exist(name)) {
          this.addComponent(name);
          return;
        }

        if (!this.configs[name]) return;

        let configs = this.configs[name];
        let usage = data.net[name];
        let date = Util.toLocalDate(data.utc);

        configs[Names.TOTAL].addWithByteDesc(date, usage.total);
        configs[Names.SENT].addWithByteDesc(date, usage.sent);
        configs[Names.RECVED].addWithByteDesc(date, usage.recved);
      }, this);

    });
  }

  ngOnDestroy() {
    if (this.websocketSubscription) {
      this.websocketSubscription.unsubscribe();
      this.websocketSubscription = null;
    }
  }

  /** DynamicComponent 추가 */
  private addComponent(name: string) {
    this.dynamic.add(
      name,
      DynamicComponent,
      {
        key: name,
        cols: 3,
        columnStyle: 'col-xs-12 col-sm-6 col-md-6 col-lg-6 col-xl-4',
        titleFormatting: () => '<h3>' + name +'</h3>',
        showTitleHRTag: true,
        showCloseButton: false,
        bottomMargin: 2
      },
      {
        onAfterViewInit: this.onAfterViewInit.bind(this)
      }
    );
  }

  public onAfterViewInit(name: string) {
    let configs = this.configs[name];
    if (!configs) {
      configs = this.configs[name] = {};
    }

    let cmp: DynamicComponent = this.dynamic.get(name);
    this.addChartComponent(configs, cmp, Names.TOTAL, ChartConfig.newByteType);
    this.addChartComponent(configs, cmp, Names.SENT, ChartConfig.newByteType);
    this.addChartComponent(configs, cmp, Names.RECVED, ChartConfig.newByteType);
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


}
