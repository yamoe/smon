import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { Routes, RouterModule } from '@angular/router';
import { LayoutComponent } from './layout.component';

const routes: Routes = [
  {
    path: '', component: LayoutComponent,
    children: [
      { path: 'cpu', loadChildren: './pages/cpu/cpu.module#CpuModule'},
      { path: 'disk', loadChildren: './pages/disk/disk.module#DiskModule'},
      { path: 'mem', loadChildren: './pages/mem/mem.module#MemModule'},
      { path: 'net', loadChildren: './pages/net/net.module#NetModule'},
      { path: 'proc', loadChildren: './pages/proc/proc.module#ProcModule'},
      { path: 'sys', loadChildren: './pages/sys/sys.module#SysModule'},
      { path: '', redirectTo: 'cpu', pathMatch: 'full'}
    ]
  },
  { path: '**', redirectTo: '' }
];

@NgModule({
  imports: [ RouterModule.forChild(routes) ],
  exports: [ RouterModule ]
})
export class LayoutRoutingModule { }
