import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { NetRoutingModule } from './net-routing.module';
import { NetComponent } from './net.component';
import { DynamicModule, DynamicComponent, ChartModule, ChartComponent } from '../../../common';

@NgModule({
  imports: [
    CommonModule,
    NetRoutingModule,
    ChartModule,
    DynamicModule
  ],
  entryComponents: [
    DynamicComponent,
    ChartComponent
  ],
  declarations: [
    NetComponent
  ]
})
export class NetModule { }

