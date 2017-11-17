import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { ProcRoutingModule } from './proc-routing.module';
import { ProcComponent } from './proc.component';
import { DynamicModule, DynamicComponent, ChartModule, ChartComponent } from '../../../common';

@NgModule({
  imports: [
    CommonModule,
    ProcRoutingModule,
    DynamicModule,
    ChartModule
  ],
  entryComponents: [
    DynamicComponent,
    ChartComponent
  ],
  declarations: [
    ProcComponent
  ]
})
export class ProcModule { }

