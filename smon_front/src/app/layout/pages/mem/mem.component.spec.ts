import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { MemComponent } from './mem.component';

describe('MemComponent', () => {
  let component: MemComponent;
  let fixture: ComponentFixture<MemComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ MemComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(MemComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
