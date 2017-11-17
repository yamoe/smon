import { Util } from '../../class/util'



/**
 * ChartComponent 를 제어하기 위한 클래스
 */
export class ChartConfig {

  private dataCount: number = 300; // 그래프에 표시할 ydata 개수(realtime 용이기에 갯수를 정함)
  private height = '130px'; // 그래프 높이
  public data = []; // graph 데이타

  public name: string;
  public description: string;


  public view = undefined;
  public scheme = { domain: ['#FF0000'] };
  public yScaleMin = undefined;
  public yScaleMax = undefined;
  public yAxisTickFormatting = function (val) { return val; };
  public tooltipFormatting = function (xdata: Date, ydata: any): string {
    return [
      'Time : ',
      Util.toDateString(xdata),
      '<br/>',
      'Usage : ',
      ydata,
    ].join('');
  };

  constructor(name: string = "", dataCount: number = 300) {
    this.name = name;
    this.dataCount = dataCount;
    this.data = [{
      "name": name,
      "series": []
    }];

    for (let i = 0; i < this.dataCount; ++i) {
      this.data[0].series.push(
        {
          'name': i,
          'value': 0,
          'date': 0,
        }
      );

    }
  }

  public getHeight() {
    return { height: this.height };
  }

  /*
  realtime 그래프에서 그래프 이동으로 인해
  tooltip에 표시되는 데이터가 갱신되지 않아
  매번 조회하도록 함수를 만듦
  */
  private getTooltipText(idx) {
    let data = this.data[0].series[idx];
    if (!(data.date instanceof Date)) return '';
    return this.tooltipFormatting(data.date, data.value);
  }

  public add(x, y) {
    let len = this.data[0].series.length;
    for (let i = 0; i < len; ++i) {
      if (i + 1 == len) break;
      let value = this.data[0].series[i];
      let next_value = this.data[0].series[i + 1];

      value.value = next_value.value;
      value.date = next_value.date;
    }
    let last_value = this.data[0].series[len - 1];
    last_value.value = y;
    last_value.date = x;

    this.data = [...this.data];
  }

  public addWith(x, y) {
    this.add(x, y);
    this.description = [' - ', y].join('');
  }

  public addWithDesc(x, y, desc) {
    this.add(x, y);
    this.description = desc;
  }

  public addWithPercentageDesc(x, y) {
    this.addWithDesc(x, y, [' - ', y, ' %'].join(''));
  }

  public addWithByteDesc(x, y) {
    this.addWithDesc(x, y, [' - ', Util.readableBytes(y)].join(''));
  }

  public addWithBytePercentageDesc(x, y, yMax) {
    this.addWithDesc(x, y, [' - ', Util.readableBytes(y), '/', Util.readableBytes(yMax), ' (', Util.percentage(y, yMax), '%)'].join(''));
  }


  static newCpuType(title: string): ChartConfig {
    let config = new ChartConfig(title, 300);
    config.yScaleMin = 0;
    config.yScaleMax = 100;

    config.tooltipFormatting = function (xdata: Date, ydata): string {
      return [
        ydata, ' %',
        '<br/>',
        Util.toDateString(xdata)
      ].join('');
    };
    return config;
  }

  static newByteType(title: string): ChartConfig {
    let config = new ChartConfig(title, 300);
    config.yAxisTickFormatting = function (bytes) {
      return Util.readableBytes(bytes);
    }

    config.tooltipFormatting = function (xdata: Date, ydata): string {
      return [
        Util.readableBytes(ydata), ' (', ydata, ' B)',
        '<br/>',
        Util.toDateString(xdata)
      ].join('');
    };

    return config;
  }

  static newCountType(title: string): ChartConfig {
    let config = new ChartConfig(title, 300);

    config.tooltipFormatting = function (xdata: Date, ydata): string {
      return [
        ydata,
        '<br/>',
        Util.toDateString(xdata)
      ].join('');
    };

    return config;
  }
}

