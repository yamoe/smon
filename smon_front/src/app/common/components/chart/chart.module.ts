import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { NgxChartsModule } from '@swimlane/ngx-charts';

import { ChartComponent } from './chart.component';

@NgModule({
    imports: [
        CommonModule,
        NgxChartsModule
    ],
    declarations: [
        ChartComponent
    ],
    exports: [
        ChartComponent
    ]
})
export class ChartModule { }
