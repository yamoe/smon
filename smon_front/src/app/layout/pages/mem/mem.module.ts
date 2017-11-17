import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { MemRoutingModule } from './mem-routing.module';
import { MemComponent } from './mem.component';
import { ChartModule } from '../../../common';

@NgModule({
  imports: [
    CommonModule,
    MemRoutingModule,
    ChartModule
  ],
  declarations: [
    MemComponent
  ]
})
export class MemModule { }
