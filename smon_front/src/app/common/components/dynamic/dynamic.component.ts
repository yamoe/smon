import { AfterViewInit, EventEmitter, Input, Output, Component, OnInit, OnDestroy, Type, ViewChildren, ViewContainerRef, QueryList, ComponentFactoryResolver } from '@angular/core';
import { DynamicCreator } from './dynamic-creator'

/** 
 * CharComponent 동적 추가 및 layout 적용을 위해 작성.
 * 다른 Component도 추가하여 사용할 수 있음.
 */
@Component({
  selector: 'app-dynamic',
  templateUrl: './dynamic.component.html'
})

export class DynamicComponent implements OnInit, AfterViewInit, OnDestroy {
  private creator: DynamicCreator;
  @ViewChildren('dynamic', { read: ViewContainerRef }) private dynamicComponents: QueryList<ViewContainerRef>;
  @ViewChildren('rows', { read: ViewContainerRef }) private rowComponents: QueryList<ViewContainerRef>;


  public rowNum = [];
  private colNum = [0];

  public topMargins = [];
  public bottomMargins = [];

  @Input() key: string; // 현재 컴포넌트의 키
  @Input() set cols(cnt: number) {  // 컬럼 div 태그 생성 개수
    this.colNum = Array(cnt).fill(0);
  }  
  @Input() columnStyle:string = 'col-md-12';  // 컬럼 div 태그의 스타일 class 설정
  @Input() titleFormatting = function() {
    return undefined;
  }
  @Input() showTitleHRTag: boolean = true;
  @Input() showCloseButton: boolean = false;  // 삭제 버튼 사용 유무
  @Input() set topMargin(cnt: number) {
    this.topMargins = Array(cnt).fill(0);
  }
  @Input() set bottomMargin(cnt: number) {
    this.bottomMargins = Array(cnt).fill(0);
  }

  @Output() onAfterViewInit = new EventEmitter<string>(true);
  @Output() onClose = new EventEmitter<string>();


  constructor(private resolver: ComponentFactoryResolver) { }

  ngOnInit() { }

  ngAfterViewInit() {
    this.creator = new DynamicCreator(
      this.resolver,
      this.dynamicComponents,
      this.rowNum,
      this.colNum,
      this.rowComponents
    );
    this.onAfterViewInit.emit(this.key);
  }

  ngOnDestroy() {
    this.creator.uninit();
    this.creator = undefined;
  }

  public exist(key: string): boolean {
    return this.creator.exist(key);
  }

  public add(key: string, componentClass: Type<any>, input: { [key: string]: any } = undefined, output: { [key: string]: any } = undefined): boolean {
    return this.creator.add(
      key,
      componentClass,
      input,
      output
    );
  }

  public get(key: string): any {
    return this.creator.get(key);
  }

  public remove(key: string): void {
    this.creator.remove(key);
  }

  public removeIfNotExist(keys: string[]): string[] {
    return this.creator.removeIfNotExist(keys);
  }

  public close(): void {
    this.onClose.emit(this.key);
  }

}

