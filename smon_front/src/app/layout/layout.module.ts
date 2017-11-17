import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { ClarityModule } from "clarity-angular";
import { LayoutRoutingModule } from './layout-routing.module';

import { HeaderAlertComponent } from './header-alert/header-alert.component';
import { HeaderComponent } from './header/header.component';
import { LayoutComponent } from './layout.component';

import { DataService } from '../common';

@NgModule({
  imports: [
    CommonModule,
    ClarityModule.forRoot(),
    LayoutRoutingModule
  ],
  declarations: [
    HeaderAlertComponent,
    HeaderComponent,
    LayoutComponent
  ],
  providers: [
    DataService
  ]
})
export class LayoutModule { }
