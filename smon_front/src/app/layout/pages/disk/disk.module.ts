import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { DiskRoutingModule } from './disk-routing.module';
import { DiskComponent } from './disk.component';
import { DynamicModule, DynamicComponent, ChartModule, ChartComponent } from '../../../common';

@NgModule({
  imports: [
    CommonModule,
    DiskRoutingModule,
    ChartModule,
    DynamicModule
  ],
  entryComponents: [
    DynamicComponent,
    ChartComponent
  ],
  declarations: [
    DiskComponent
  ],
})
export class DiskModule { }

