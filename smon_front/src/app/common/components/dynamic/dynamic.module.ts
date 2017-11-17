import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { ClarityModule } from "clarity-angular";

import { DynamicComponent } from './dynamic.component';

@NgModule({
    imports: [
        CommonModule,
        ClarityModule.forRoot()
    ],
    declarations: [
        DynamicComponent
    ],
    exports: [
        DynamicComponent
    ]
})
export class DynamicModule { }
