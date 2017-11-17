import { ComponentRef, Type, QueryList, Inject, ComponentFactoryResolver, ViewContainerRef } from '@angular/core';
import { Subscription } from 'rxjs/Subscription';

/**
 * Component 동적 생성 및 관리를 위한 클래스
 * DynamicComponent와 DoubleDynamicComponet 에서 사용
 * 
 * 참고 : row(div tag) 동적 추가시 바로 쓸수 없으므로
 * waitingCmp 에서 생성할 컴포넌트 정보를 갖고 있다가
 * rows subscribe 를 통해 rendering 완료되면
 * 실제 component를 생성하여 추가함.
 */

export class ComponentCreator {

    private views: { [key: string]: ViewContainerRef } = {};
    private cmps: { [key: string]: ComponentRef<any> } = {};
    private waitingCmps: { [key: string]: any[] } = {};   // value : input for component

    private subscription: Subscription;

    constructor(
        private resolver: ComponentFactoryResolver,
        private components: QueryList<ViewContainerRef>,
        private componentType: Type<any>,
        private rows: any[],
        private cols: any[],
        rowsComponent: QueryList<any>
    )
    {
        this.subscribeChange(rowsComponent);
    }

    public exist(key: string): boolean {
        if (this.waitingCmps[key]) return true;
        if (this.views[key]) return true;
        return false;
    }

    /**
     * 컴포넌트가 추가될 rows 에 변화가 생기는지 감시하다가
     * 추가 대기중인 컴포넌트가 있는 경우 추가
     * 
     * @param rowsComponent 컴포넌트가 추가될 rows component
     */
    private subscribeChange(rowsComponent: QueryList<any>) {
        this.subscription = rowsComponent.changes.subscribe(t => {
            setTimeout(() => {
                Object.keys(this.waitingCmps).forEach(function (key) {
                    let input = this.waitingCmps[key][0];
                    let output = this.waitingCmps[key][1];

                    if (this.hasRoom()) {
                        this.addComponent(key, input, output);
                        delete this.waitingCmps[key];
                    } else {
                        this.addRoom();
                    }
                }, this);
            }, this);
        });
    }

    public add(key: string, input: { [key: string]: any } = undefined, output: { [key: string]: any } = undefined): boolean {
        if (this.exist(key)) return false;
        
        if (!this.hasRoom()) {
            this.addRoom();
            this.waitingCmps[key] = [input, output];
            return true;
        }

        return this.addComponent(key, input, output);
    }

    public get(key: string) {
        let cmp = this.cmps[key];
        if (cmp) return cmp.instance;
        return undefined;
    }

    public remove(key: string) {
        if (this.waitingCmps[key]) {
            delete this.waitingCmps[key];
        }
      
        let view = this.views[key];
        if (view) {
            view.clear();
            delete this.views[key];

            this.cmps[key].destroy();
            delete this.cmps[key];
        }
    }

    /** keys 에 없는 component 삭제 및 삭제된 key 리턴 */
    public removeIfNotExist(keys: string[]) : string[] {
        let deletedKeys: string[] = [];
        Object.keys(this.views).forEach(function (key) {
            if (!keys.includes(key)) {
                this.remove(key);
                deletedKeys.push(key);
            }
        }, this);
        return deletedKeys;
    }

    /** Component에선 OnDestroy에서 이 함수를 반드시 호출해야 함(자원 해제) */
    public uninit() {
        if (this.subscription) this.subscription.unsubscribe();
    }

    private hasRoom() : boolean {
        return (this.components.length - Object.keys(this.cmps).length) > 0;
    }

    private addRoom(): void {
        if (this.isCreatingRoom()) return;
        this.rows.push(0);
    }

    private isCreatingRoom(): boolean {
        return (this.rows.length * this.cols.length) > this.components.length;
    }

    private addComponent(key: string, input: { [key: string]: any } = undefined, output:{ [key: string] : any } = undefined): boolean {
        // add component
        let arr = this.components.toArray();
        
        for (let i = 0; i < arr.length; ++i) {
            let target = arr[i];

            if (target.length === 0) {
                
                const factory = this.resolver.resolveComponentFactory(this.componentType);
                const componentRef = target.createComponent(factory);

                // component input        
                if (input) {
                    Object.keys(input).forEach(function (key) {
                        let value = input[key];
                        eval('componentRef.instance.' + key + ' = value');
                    }, this);
                }

                //component output
                if (output) {
                    Object.keys(output).forEach(function (key) {
                        let value = output[key];

                        eval('componentRef.instance.' + key + '.subscribe(value)');
                    }, this);
                }

                this.views[key] = target;
                this.cmps[key] = componentRef;
                return true;
            }
        }

        // 이 경우는 발생하면 안되도록 작성하였으나
        // 발생하는 경우 감지를 위해 에러 로깅 추가
        console.error('fail add component : ' + key);
        return false;
    }

}
