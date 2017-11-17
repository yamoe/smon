import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { CpuRoutingModule } from './cpu-routing.module';
import { CpuComponent } from './cpu.component';
import { DynamicModule, DynamicComponent, ChartModule, ChartComponent } from '../../../common';

@NgModule({
  imports: [
    CommonModule,
    CpuRoutingModule,
    ChartModule,
    DynamicModule
  ],
  entryComponents: [
    DynamicComponent,
    ChartComponent
  ],
  declarations: [
    CpuComponent
  ]
})
export class CpuModule { }

