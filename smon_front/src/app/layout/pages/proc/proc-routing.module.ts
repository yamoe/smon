import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { ProcComponent } from './proc.component';

const routes: Routes = [
    { path: '', component: ProcComponent }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class ProcRoutingModule { }
