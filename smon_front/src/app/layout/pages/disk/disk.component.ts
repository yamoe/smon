import { Component, ViewChild, OnInit, OnDestroy } from '@angular/core';
import { DataService, ChartConfig, ChartComponent, DynamicComponent, DiskData, Util } from '../../../common';
import { Subscription } from 'rxjs/Subscription';

const Names = {
  USE: 'use',
  IO_TOTAL: 'io total',
  IO_READ: 'io read',
  IO_WRITE: 'io write'
};

@Component({
  selector: 'app-disk',
  template: `
    <div class="row">
      <div class="col-md-12">
        <app-dynamic #dynamic></app-dynamic>
      </div>
    </div>
  `
})

export class DiskComponent implements OnInit, OnDestroy {
  // key1 : disk name, key2 : Names, value : CharConfig>>
  private configs: { [key: string]: { [key: string]: ChartConfig } } = {};
  private websocketSubscription: Subscription;
  @ViewChild('dynamic') private dynamic: DynamicComponent;

  constructor(private dataService: DataService) { }

  ngOnInit() {
    this.websocketSubscription = this.dataService.resDisk().subscribe(data => {
      
      let date = Util.toLocalDate(data.utc);
      let keys = Object.keys(data.disk);

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
        let usage = data.disk[name];

        // add data
        let useConfig = configs[Names.USE];
        if (!useConfig.yScaleMax) {
          useConfig.yScaleMax = usage.total
        }
        useConfig.addWithBytePercentageDesc(date, usage.use, usage.total);
        configs[Names.IO_TOTAL].addWithByteDesc(date, usage.ioTotal);
        configs[Names.IO_READ].addWithByteDesc(date, usage.ioRead);
        configs[Names.IO_WRITE].addWithByteDesc(date, usage.ioWrite);

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
        cols: 4,
        columnStyle: 'col-xs-12 col-sm-6 col-md-6 col-lg-6 col-xl-3',
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

  /** ChartComponent 추가 */
  public onAfterViewInit(name: string) {
    let configs = this.configs[name];
    if (!configs) {
      configs = this.configs[name] = {};
    }

    let cmp: DynamicComponent = this.dynamic.get(name);
    this.addChartComponent(configs, cmp, Names.USE, ChartConfig.newByteType);
    this.addChartComponent(configs, cmp, Names.IO_TOTAL, ChartConfig.newByteType);
    this.addChartComponent(configs, cmp, Names.IO_READ, ChartConfig.newByteType);
    this.addChartComponent(configs, cmp, Names.IO_WRITE, ChartConfig.newByteType);
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
