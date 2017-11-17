import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { SysRoutingModule } from './sys-routing.module';
import { SysComponent } from './sys.component';

@NgModule({
  imports: [
    CommonModule,
    SysRoutingModule
  ],
  declarations: [
    SysComponent
  ]
})
export class SysModule { }
