import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { NgModule } from '@angular/core';
import { RouteReuseStrategy } from '@angular/router';

import { AppComponent } from './app.component';
import { AppRoutingModule } from './app-routing.module';
import { CustomReuseStrategy } from './common';

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    AppRoutingModule
  ],
  providers: [
    { provide: RouteReuseStrategy, useClass: CustomReuseStrategy }
  ],
  bootstrap: [ AppComponent ]
})
export class AppModule { }
