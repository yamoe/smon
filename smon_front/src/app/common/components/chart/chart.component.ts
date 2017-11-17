import { Component, Input } from '@angular/core';
import { ChartConfig } from './chart-config';

@Component({
  selector: 'app-chart',
  templateUrl: './chart.component.html'
})

export class ChartComponent {
  @Input() config: ChartConfig;

  constructor() {}
}
