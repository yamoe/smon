import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { CpuComponent } from './cpu.component';

const routes: Routes = [
    { path: '', component: CpuComponent }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class CpuRoutingModule { }
