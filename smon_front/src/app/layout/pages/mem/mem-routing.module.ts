import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { MemComponent } from './mem.component';

const routes: Routes = [
    { path: '', component: MemComponent }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class MemRoutingModule { }
