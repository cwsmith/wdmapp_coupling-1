#include "dataprocess.h"
#include "dataprocess_impl.h"
#include "importpart3mesh.h"
#include "commpart1.h"

namespace coupler {

void DatasProc3D::init(const Part1ParalPar3D& p1pp3d, const Part3Mesh3D &p3m3d)
{
  if(preproc==true){
    if(yparal==true){
      if(p1pp3d.li0%p1pp3d.npy==0){
        part1li0=p1pp3d.li0/p1pp3d.npy;
        part3li0=part1li0;
      } else{
        if(p1pp3d.mype_y==p1pp3d.npy-1){
          part1li0=p1pp3d.li0%p1pp3d.npy;
          part3li0=part1li0;
        }  else{
          part1li0=p1pp3d.li0%p1pp3d.npy;
          part3li0=part1li0;
        }
      }
      part1lj0=2*p1pp3d.ny0;
      part3lj0=p1pp3d.ny0;   // here may need rethinking.
    } else{
      part1li0=p1pp3d.li0;
      part3li0=p1pp3d.li0;
      part1lj0=2*p1pp3d.ny0;
      part3lj0=p1pp3d.ny0;   // here may need rethinking.
    }
  }
  for(LO i=0;i<p3m3d.li0;i++)  sum+=p3m3d.mylk0[i];
}

void DatasProc3D::AllocDensityArrays(const Part1ParalPar3D& p1pp3d, const Part3Mesh3D& p3m3d)
{
  if(yparal==false){
    densin=new CV**[p1pp3d.li0];
    for(LO i=0;i<p1pp3d.li0;i++){
      densin[i]=new CV*[p1pp3d.lj0];
      for(LO j=0;j<p1pp3d.lj0;j++)
        densin[i][j]=new CV[p1pp3d.lk0];
    }
    GO num=p1pp3d.li0*p1pp3d.lj0*p1pp3d.lk0;
    densintmp=new CV[num];

    num=p3m3d.li0*p3m3d.lj0*p1pp3d.lk0;
    densouttmp=new double[num];

    densout=new double**[p1pp3d.li0];
    for(LO i=0;i<p3m3d.li0;i++){
      densout[i]=new double*[p1pp3d.lj0*2];
      for(GO j=0;j<p3m3d.lj0;j++){
        densout[i][j]=new double[p1pp3d.lk0];
      }
    }

    denspart3=new double**[p3m3d.li0];
    for(LO i=0;i<p3m3d.li0;i++){
      denspart3[i]=new double*[p3m3d.lj0];
      for(LO j=0; j<p3m3d.lj0; j++)
        denspart3[i][j]=new double[p3m3d.mylk0[i]];
    }
  } 
}

void DatasProc3D::AllocPotentArrays(const Part1ParalPar3D& p1pp3d, const Part3Mesh3D& p3m3d)
{
  if(yparal==false){
    potentin=new double**[p3m3d.li0];
    for(LO i=0;i<p3m3d.li0;i++){
      potentin[i]=new double*[p3m3d.lj0];
      for(LO j=0;j<p3m3d.lj0;j++)
        potentin[i][j]=new double[p3m3d.mylk0[i]];
    }
    potentintmp=new double[sum*p3m3d.lj0];
    /*  for(LO k=0;k<sum;k++){
        potenttmp[k]=new double[p3m3d.lj0];
        }
        */
    potentouttmp=new CV[sum*p3m3d.lj0/2];
    potentout=new CV**[p3m3d.li0];
    for(LO i=0;i<p3m3d.li0;i++){
      potentout[i]=new CV*[part3lj0];
      for(LO j=0;j<p3m3d.lj0;j++)
        potentout[i][j]=new CV[p3m3d.mylk0[i]];
    }

    potentpart1=new CV**[p1pp3d.li0];
    for(LO i=0;i<p1pp3d.li0;i++){
      potentpart1[i]=new CV*[p1pp3d.lj0];
      for(LO j=0;j<p1pp3d.lj0;j++){
        potentpart1[i][j]=new CV[p1pp3d.lk0];
      }
    }
  }
}

DatasProc3D::~DatasProc3D()
{
  FreeFourierPlan3D();
  if(densin!=NULL) delete[] densin;
  if(densintmp!=NULL) delete[] densintmp;
  if(densouttmp!=NULL) delete[] densouttmp;
  if(densout!=NULL) delete[] densout;
  if(denspart3!=NULL) delete[] denspart3;
  if(potentin!=NULL) delete[] potentin;
  if(potentintmp!=NULL) delete[] potentintmp;
  if(potentouttmp!=NULL) delete[] potentouttmp;
  if(potentout!=NULL) delete[] potentout;
  if(potentpart1!=NULL) delete[] potentpart1;       
}

}
