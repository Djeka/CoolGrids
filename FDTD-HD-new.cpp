
#include "GridsCommon.hpp"
#include "intel_ode.h"

using namespace std;


const double PI_A0_SQR = 0.88E-16     *1E-4;// [м^2]
const double Ry=13.6;// [эВ]

const double I = 14.86;// Константа I в ф-ле для сечения ионизации, эВ

const double MU = 1.0;


//Глобальные физические константы
const double EPS0=8.854E-12;// Ф/м
const double MU0=1.256E-6;// Гн/м
const double SPEED_OF_LIGHT=299792458.0;// [м/с]
const double VACUUM_WAVE_RESISTIVITY=sqrt(MU0/EPS0);// волновое сопротивление вакуума [Ом]

const double C=SPEED_OF_LIGHT;// м/с    C=sqrt(1/EPS0/MU0)

const double ATOMIC_MASS_UNIT = 1.660538782E-27;// [кг]
const double AVERAGE_AIR_ION_MASS = 29.0 * ATOMIC_MASS_UNIT;// [кг]  для высот < 100-120 км молекулярная масса почти постоянна
const double ELECTRON_MASS = 9.10938215E-31;// Масса электрона, [кг]


//параметры атмосферы на высоте 40км
const double ATMOSPHERE_O2_CONCENTRATION_40KM = 1.7E16         * 1E6;// [1/м^3]
const double ATMOSPHERE_ELECTRON_CONCENTRATION_40KM = 3.16E4   * 1E6;// [1/м^3]
const double ATMOSPHERE_TEMPERATURE_40KM = 250.0;// [К]
const double ATMOSPHERE_DENSITY_40KM = 0.003851;// [кг/м^3]


const double Wevlt = 1.602176487E-19;// Джоулей в одном э­лектрон-Вольте

const double cs = SPEED_OF_LIGHT;//Скороcть cвета, m/s [cs]

const double cze = 1.602E-19;//Заряд электрона, Кл. [cze]
const double cme = 9.10938215E-31;//Масса электрона, кг ? [cme]
const double ckb = 0.861E-4;//Константа Больцмана, э­В/Кград [ckb]
const double ci = 14.86;//Константа I в ф-ле для сечения ионизации, эВ [ci]
const double cih = 13.6;//Консттанта Ih в ф-ле для сечения ионизации, эВ [cih]
const double ca2 = 3.52E-16;//Константа 4*Pi*alfa2 в ф-ле для сечения ионизации, sm2 [ca2]
const double ce0 = 0.025;//Константа e0 в ф-ле для Qei, эВ [ce0]
const double cnw = 1.14e23;//Концентрация воздуха у поверхности Земли, 1/м3 ? [cnw]
const double cbv = -0.348E-4;//Показатель компоненты в распр. концентрации воздуха по высоте, ? [cbv]

const double cno2 = 0.21;//Доля кислорода в атмосфере, ? [cno2]

const double czi = 900000.0;//Высота начала ионосферы, м ? [czi]   60-90км

const double czb = 1.0;//Степень ионизации [czb]

const double cne0 = 3E4;//28E6;//начальная концентрация электронов, 1/см3 ? [cne0]
const double cni0 = cni0;//28E6;//начальная концентрация ионов, 1/см3 ? [cni0]

//!!! НЕ ИСПОЛЬЗУЕТСЯ! double ct0=200000.0;// ЖЕСТКОВАТО!!! 1000. 115070.   Температура воздуха у поверхности Земли, гр.К [ct0]
//!!!double cti=200000.0;//  1000. 115070.   наачальная температура электронов, гр.К [cti] TODO какова на самом деле температура электронов в атмосфере
const double chi = 4E9;//  8 5.5e8 !!!!!0.55e9 !!!3.e9          Опорная частота (Гц) [chi]
const double csi = 4E5;//  1.e8 1.5e8 !!!1.e6    Частота следований импульсов (скважность) (Гц) [csi]
const double cpi = 1.0;//  25 50 5  !!!!!3 1 !!!50.                Число импульсов в "пачке" [cpi]
const double cep = 1E7;//  !!!5.e5 !!!1.e7 1.e4       амплитуда напряженности электрического поля (В/м) [cep]
const double Z0 = 42000.0;//  40000. !!!50000. Nachalnaya vysota   176121372031.662 [Z0]

const double WPI=M_PI;
const double WZ = 1.0;

const double WM = AVERAGE_AIR_ION_MASS;//= 4.84d-26 ! average air ion mass = ??????? in SI
const double WEt0 = 0.025;
const double Wmu0 = 4*WPI*1E-7;


double k_b=1.38E-23;// Дж/К

double NE0=ATMOSPHERE_ELECTRON_CONCENTRATION_40KM;
double ATM_T=200.0; //температура атмоcферы K

void rhs(int *ptrN,double *t,double *u,double *f);



struct UserData{
	double E_x;
	double E_y;
	double E_z;

	double H_x;
	double H_y;
	double H_z;

	double V_SQR;
};


//accumulator acc_U0;
//accumulator acc_U1;
//accumulator acc_U2;
//accumulator acc_U3;
//accumulator acc_U4;
//
//accumulator acc_f0;
//accumulator acc_f1;
//accumulator acc_f2;
//accumulator acc_f3;
//accumulator acc_f4;

double dt;

int main(){

//	accumulator acc;
//	acc(10.0);
//	acc(8.0);
//	acc(13.0);
//	acc(9.0);
//	acc(11.0);
//	acc(14.0);
//	acc(6.0);
//	acc(4.0);
//	acc(12.0);
//	acc(7.0);
//	acc(5.0);
//
//	printStats(acc);


	const int ODE_SOLVE_IPAR_SIZE=128;
	int ipar[ODE_SOLVE_IPAR_SIZE];
	for(int i=0;i<ODE_SOLVE_IPAR_SIZE;i++)ipar[i]=0;
	ipar[0] = 0;
	ipar[1] = 0;//scheme auto-choose
	ipar[2] = 0;//0 - exit at the end ; 1 - exit after every step
	ipar[3] = 0;//0 - autocalc Jacobi matrix; 1 - user-defined Jacobi matrix
	ipar[4] = 0;//0 - don't freeze Jacobi matrix; 1 - freeze Jacobi matrix
	ipar[5] = 0;
	ipar[6] = 0;
	ipar[7] = 0;//TODO move into separate function

	omp_set_num_threads(2);

	VtiSaver3D vtiSaver;
	CImgSaver2D cimgSaver;


    double Nx=256;
    double Ny=257;
    double Nz=20;
    double Nt=2000;

    double SOURCE_IX = (int)(Nx*0.1)+0.5;
    double SOURCE_IY = (int)(Ny*0.5)+0.5;
    double SOURCE_IZ = Nz/2;

    double IMPULSE_FREQ = 3e8;
    double IMPULSE_TIME = 1.0/IMPULSE_FREQ;
    double IMPULSE_LENGTH = C*IMPULSE_TIME;

    double E_AMPLITUDE=1e6;
    double H_AMPLITUDE=E_AMPLITUDE/VACUUM_WAVE_RESISTIVITY;

	cimgSaver.setValueRange(-2.0*H_AMPLITUDE , 2.0*H_AMPLITUDE);

	/*
    double Sx=10.0;
	double Sy= (Sx/Nx) * Ny;
	double Sz= (Sx/Nx) * Nz;
	double T=1.0;

	double dx=Sx/Nx;
	double dy=Sy/Ny;
	double dz=Sz/Nz;
	*/
	//double DT=T/Nt;




	double dx,dy,dz;
	dx=dy=dz = IMPULSE_LENGTH/32.0;

	double Sx= dx*Nx;
	double Sy= dy*Ny;
	double Sz= dz*Nz;
	double T=1.0;

    double DT = 0.5*dx/C;
    dt = DT;//TODO remove global dt


    std::cout << "stability parameters:" << std::endl;

    CHECK(C*DT/dx,<,1/sqrt(3));
    CHECK(IMPULSE_TIME/DT,>=,32);
    CHECK(IMPULSE_LENGTH/dx,>=,32);


	TimedGrid3D<double> Ex("Ex");
	TimedGrid3D<double> Ey("Ey");
	TimedGrid3D<double> Ez("Ez");

	TimedGrid3D<double> Hx("Hx");
	TimedGrid3D<double> Hy("Hy");
	TimedGrid3D<double> Hz("Hz");


    TimedGrid3D<double> epsilon("epsilon");
	TimedGrid3D<double> mu("mu");
	TimedGrid3D<double> sigma("sigma");




	//building grids
	Ex.setRangeT(0,T);
	Ex.setRangeX(0.5*dx , Sx-0.5*dx);
	Ex.setRangeY(0,Sy);
	Ex.setRangeZ(0,Sz);

	Ex.setIndexRangeT(0,Nt);
	Ex.setIndexRangeX(0.5 , Nx-0.5);
	Ex.setIndexRangeY(0,Ny);
	Ex.setIndexRangeZ(0,Nz);

	Ex.setLayersCountToMaintain(5);
	Ex.build();


	Ey.setRangeT(0,T);
	Ey.setRangeX(0 , Sx);
	Ey.setRangeY(0.5*dy , Sy-0.5*dy);
	Ey.setRangeZ(0 , Sz);

	Ey.setIndexRangeT(0,Nt);
	Ey.setIndexRangeX(0,Nx);
	Ey.setIndexRangeY(0.5,Ny-0.5);
    Ey.setIndexRangeZ(0,Nz);

    Ey.setLayersCountToMaintain(5);
	Ey.build();


	Ez.setRangeT(0,T);
	Ez.setRangeX(0 , Sx);
	Ez.setRangeY(0 , Sy);
	Ez.setRangeZ(0.5*dz , Sz-0.5*dz);

	Ez.setIndexRangeT(0,Nt);
	Ez.setIndexRangeX(0 , Nx);
	Ez.setIndexRangeY(0 , Ny);
	Ez.setIndexRangeZ(0.5 , Nz-0.5);

	Ez.setLayersCountToMaintain(5);
	Ez.build();



	Hx.setRangeT(0,T);
	Hx.setRangeX(0 , Sx);
	Hx.setRangeY(0.5*dy , Sy-0.5*dy);
	Hx.setRangeZ(0.5*dz , Sz-0.5*dz);

	Hx.setIndexRangeT(0.5,Nt+0.5);
	Hx.setIndexRangeX(0   , Nx);
	Hx.setIndexRangeY(0.5 , Ny-0.5);
	Hx.setIndexRangeZ(0.5 , Nz-0.5);

	Hx.setLayersCountToMaintain(2);
	Hx.build();




	Hy.setRangeT(0,T);
	Hy.setRangeX(0.5*dx , Sx-0.5*dx);
	Hy.setRangeY(0      , Sy       );
	Hy.setRangeZ(0.5*dz , Sz-0.5*dz);

	Hy.setIndexRangeT(0.5,Nt+0.5);
	Hy.setIndexRangeX(0.5 , Nx-0.5);
	Hy.setIndexRangeY(0   , Ny    );
    Hy.setIndexRangeZ(0.5 , Nz-0.5);

    Hy.setLayersCountToMaintain(2);
	Hy.build();




	Hz.setRangeT(0,T);
	Hz.setRangeX(0.5*dx , Sx-0.5*dx);
	Hz.setRangeY(0.5*dy , Sy-0.5*dy);
	Hz.setRangeZ(0      , Sz);

	Hz.setIndexRangeT(0.5,Nt+0.5);
	Hz.setIndexRangeX(0.5 , Nx-0.5);
	Hz.setIndexRangeY(0.5 , Ny-0.5);
	Hz.setIndexRangeZ(0   , Nz    );

	Hz.setLayersCountToMaintain(2);
	Hz.build();



	epsilon.setRangeT(0,T);
	epsilon.setRangeX(0,Sx);
	epsilon.setRangeY(0,Sy);
	epsilon.setRangeZ(0,Sz);

	epsilon.setIndexRangeT(0,Nt);
	epsilon.setIndexRangeX(0,Nx);
	epsilon.setIndexRangeY(0,Ny);
	epsilon.setIndexRangeZ(0,Nz);

	epsilon.build();

	epsilon[0].iterateWhole(GRID3D_ITERATOR{
	    epsilon(0,ix,iy,iz) = 1.0;
	});


	mu.setRangeT(0,T);
	mu.setRangeX(-0.5*dx,Sx+0.5*dx);
	mu.setRangeY(-0.5*dy,Sy+0.5*dy);
	mu.setRangeZ(-0.5*dz,Sz+0.5*dz);

	mu.setIndexRangeT(0,Nt);
	mu.setIndexRangeX(-0.5,Nx+0.5);
	mu.setIndexRangeY(-0.5,Ny+0.5);
	mu.setIndexRangeZ(-0.5,Nz+0.5);

	mu.build();

	mu[0].iterateWhole(GRID3D_ITERATOR{
	    mu(0,ix,iy,iz)=1.0;
	});


    //#define PRYSM
    #ifdef PRYSM
	epsilon[0].fillAntialiased(1.5,GRID3D_CONDITION{
		return (ix+iy > 1.2*avg(Nx,Ny)) && (ix < 0.8*Nx) && (iy < 0.8*Ny);
	});

	mu[0].fillAntialiased(1.5,GRID3D_CONDITION{
		return (ix+iy > 1.2*avg(Nx,Ny)) && (ix < 0.8*Nx) && (iy < 0.8*Ny);
	});
    #endif




	sigma.setRangeT(0,T);
	sigma.setRangeX(0,Sx);
	sigma.setRangeY(0,Sy);
	sigma.setRangeZ(0,Sz);

	sigma.setIndexRangeT(0,Nt);
	sigma.setIndexRangeX(0,Nx);
	sigma.setIndexRangeY(0,Ny);
	sigma.setIndexRangeZ(0,Nz);

	sigma.build();

	sigma[0].iterateWhole(GRID3D_ITERATOR{
	    sigma(0,ix,iy,iz)=0.0;
	});

	vtiSaver.save(epsilon[0],"epsilon.vti");
	vtiSaver.save(mu[0],"mu.vti");



	TimedGrid3D < Vector5D > U("U");
    TimedGrid3D < Vector5D > Fx("Fx");
	TimedGrid3D < Vector5D > Fy("Fy");
	TimedGrid3D < Vector5D > Fz("Fz");

    U.setRangeT(0, T);
	U.setRangeX(0.5 * dx, Sx - 0.5 * dx);
	U.setRangeY(0.5 * dy, Sy - 0.5 * dy);
	U.setRangeZ(0.5 * dz, Sz - 0.5 * dz);

    U.setIndexRangeT(0, Nt);
	U.setIndexRangeX(0.5, Nx - 0.5);
	U.setIndexRangeY(0.5, Ny - 0.5);
	U.setIndexRangeZ(0.5, Nz - 0.5);
	U.build();
	U.fill(Vector5D(5));

    Fx.setRangeT(0, T);
	Fx.setRangeX(0.0 * dx, Sx);
	Fx.setRangeY(0.5 * dy, Sy - 0.5 * dy);
	Fx.setRangeZ(0.5 * dz, Sz - 0.5 * dz);

    Fx.setIndexRangeT(0, Nt);
	Fx.setIndexRangeX(0, Nx);
	Fx.setIndexRangeY(0.5, Ny - 0.5);
	Fx.setIndexRangeZ(0.5, Nz - 0.5);
	Fx.build();
	Fx.fill(Vector5D(5));

    Fy.setRangeT(0, T);
	Fy.setRangeX(0.5 * dx, Sx - 0.5 * dx);
	Fy.setRangeY(0.0 * dy, Sy);
	Fy.setRangeZ(0.5 * dz, Sz - 0.5 * dz);

    Fy.setIndexRangeT(0, Nt);
	Fy.setIndexRangeX(0.5, Nx - 0.5);
	Fy.setIndexRangeY(0, Ny);
	Fy.setIndexRangeZ(0.5, Nz - 0.5);
	Fy.build();
	Fy.fill(Vector5D(5));

    Fz.setRangeT(0, T);
	Fz.setRangeX(0.5 * dx, Sx - 0.5 * dx);
	Fz.setRangeY(0.5 * dy, Sy - 0.5 * dy);
	Fz.setRangeZ(0.0, Sz);

    Fz.setIndexRangeT(0, Nt);
	Fz.setIndexRangeX(0.5, Nx - 0.5);
	Fz.setIndexRangeY(0.5, Ny - 0.5);
	Fz.setIndexRangeZ(0, Nz);
	Fz.build();
	Fz.fill(Vector5D(5));



    U[0].iterateWhole(GRID3D_ITERATOR{
	         U(0,ix,iy,iz) = HdVec3D::fromDensityPressureVelocity(NE0*cme , 3.0/2.0*k_b*ATM_T*NE0 , 0 ,0 ,0);
	});





	Timer mainTimer;
		for(double it=0;it<=Nt;it++){
			mainTimer.logTime("calculating layer "+ toString(it));


			Ex[it+1];
			Ey[it+1];
			Ez[it+1];
			Hx[it+1.5];
			Hy[it+1.5];
			Hz[it+1.5];


	        cout << "source\n" << endl;
			double t = it * DT;


	        double sourceHz = H_AMPLITUDE*sin(2.0*M_PI*t/IMPULSE_TIME)*gaussStep(t,IMPULSE_TIME*4,IMPULSE_TIME);


	        //Hz(it+0.5, SOURCE_IX , SOURCE_IY , SOURCE_IZ) = sourceHz;
	        cout << "sourceHz=" << sourceHz << endl;


	        //double KZ=1;
	        //double KY=0.5;


	        double KZ = 0;
	        double KY = 0;

	        int PAD_SIZE_Y = 100;
	        int PAD_SIZE_Z = 5;

	        for(double iz=SOURCE_IZ-PAD_SIZE_Z;iz<=SOURCE_IZ+PAD_SIZE_Z;iz++)
	            for(double iy=SOURCE_IY-PAD_SIZE_Y;iy<=SOURCE_IY+PAD_SIZE_Y;iy++){

	                double DZ= (iz-SOURCE_IZ)*dz;
	                double DY= (iy-SOURCE_IY)*dy;

	                double DR = length(DZ,DY);

	                //double sourceHz=H_AMPLITUDE*sin(2.0*M_PI*  (t/IMPULSE_TIME_WIDTH - KY*DY - KZ*DZ)) *gaussStep(t,IMPULSE_TIME_WIDTH*4,IMPULSE_TIME_WIDTH);
	                sourceHz=H_AMPLITUDE * onePlusCosPulse(DR/(dx*length(PAD_SIZE_Y,PAD_SIZE_Z)) *M_PI) *sin(2.0*M_PI*  (t/IMPULSE_TIME - KY*DY - KZ*DZ)) *gaussStep(t,IMPULSE_TIME*1.5,IMPULSE_TIME);
	                //sourceHz=H_AMPLITUDE *sin(2.0*M_PI*  (t/IMPULSE_TIME - KY*DY - KZ*DZ)) *gaussStep(t,IMPULSE_TIME*1.5,IMPULSE_TIME);

	                //sourceHz=H_AMPLITUDE * cosPulse(DR/(dx*length(PAD_SIZE_Y,PAD_SIZE_Z)) * 0.5*M_PI) *sin(2.0*M_PI*  (t/IMPULSE_TIME - KY*DY - KZ*DZ)) *gaussStep(t,IMPULSE_TIME*1.5,IMPULSE_TIME);


	                Hz(it+0.5, SOURCE_IX , iy , iz) = sourceHz;
	            }







	        cout << "Ex" << endl;
	        Ex[it+1].iterateInternal(0,1,1,GRID3D_ITERATOR{
	            double EPS = avg(epsilon(0,ix-0.5,iy,iz),epsilon(0,ix+0.5,iy,iz));
	            double SIGMA = avg(sigma(0,ix-0.5,iy,iz),sigma(0,ix+0.5,iy,iz));

	            /*
	            Ex(it + 1, ix, iy, iz) =

	                (1.0-SIGMA*DT/2.0/EPS/EPS0)/(1.0+SIGMA*DT/2.0/EPS/EPS0)*Ex(it, ix, iy, iz)
	                + DT/(1.0+SIGMA*DT/2.0/EPS/EPS0)/EPS/EPS0*(
	                (Hz(it+0.5, ix, iy+0.5, iz) - Hz(it+0.5, ix, iy - 0.5, iz)) / dy
	                -
	                (Hy(it+0.5, ix, iy, iz+0.5) - Hy(it+0.5, ix, iy, iz - 0.5)) / dz
	                );
	            */

				const double n_e = avg( HdVec3D::density(U(it,ix,iy-0.5,iz-0.5)) , HdVec3D::density(U(it,ix,iy-0.5,iz+0.5)) , HdVec3D::density(U(it,ix,iy+0.5,iz-0.5)) , HdVec3D::density(U(it,ix,iy+0.5,iz+0.5)) ); // [1/m^3]
				const double V_x = avg( HdVec3D::velocityX(U(it,ix,iy-0.5,iz-0.5)) , HdVec3D::velocityX(U(it,ix,iy-0.5,iz+0.5)) , HdVec3D::velocityX(U(it,ix,iy+0.5,iz-0.5)) , HdVec3D::velocityX(U(it,ix,iy+0.5,iz+0.5)) );

	            const double jx = cze * V_x * n_e;// [A/м^2]

	            Ex(it + 1, ix, iy, iz) = Ex(it, ix, iy, iz) +
	            		DT/EPS0/EPS*( (Hz(it+0.5, ix, iy+0.5, iz) - Hz(it+0.5, ix, iy - 0.5, iz)) / dy
	            				       -
	            				       (Hy(it+0.5, ix, iy, iz+0.5) - Hy(it+0.5, ix, iy, iz - 0.5)) / dz
	            				       - jx
		                );//TODO сравнить j и rot(H)

	        });

	        cout << "Ey" << endl;
	        Ey[it+1].iterateInternal(1,0,1,GRID3D_ITERATOR{
	            double EPS = avg(epsilon(0,ix,iy-0.5,iz),epsilon(0,ix,iy+0.5,iz));
	            double SIGMA=avg(sigma(0,ix,iy-0.5,iz),sigma(0,ix,iy+0.5,iz));

	            /*
	            Ey(it+1, ix, iy, iz)=
	                (1.0-SIGMA*DT/2.0/EPS/EPS0)/(1.0+SIGMA*DT/2.0/EPS/EPS0)*Ey(it, ix, iy, iz)
	                + DT/(1.0+SIGMA*DT/2.0/EPS/EPS0)/EPS/EPS0 * (
	                (Hx(it+0.5, ix, iy, iz+0.5) - Hx(it+0.5, ix, iy, iz-0.5)) / dz
	                -
	                (Hz(it+0.5, ix+0.5, iy, iz) - Hz(it+0.5, ix-0.5, iy, iz)) / dx
	                );
	            */

	            const double n_e = avg( HdVec3D::density(U(it,ix-0.5,iy,iz-0.5)), HdVec3D::density(U(it,ix-0.5,iy,iz+0.5)) , HdVec3D::density(U(it,ix+0.5,iy,iz-0.5)) , HdVec3D::density(U(it,ix+0.5,iy,iz+0.5))); // [1/m^3]
	            const double V_y = avg( HdVec3D::velocityY(U(it,ix-0.5,iy,iz-0.5)) , HdVec3D::velocityY(U(it,ix-0.5,iy,iz+0.5)) , HdVec3D::velocityY(U(it,ix+0.5,iy,iz-0.5)) , HdVec3D::velocityY(U(it,ix+0.5,iy,iz+0.5)));

	            const double jy = cze * V_y * n_e;// [A/м^2]

	            Ey(it+1, ix, iy, iz) = Ey(it, ix, iy, iz) +
	            		DT/EPS0/EPS * (
	        	                (Hx(it+0.5, ix, iy, iz+0.5) - Hx(it+0.5, ix, iy, iz-0.5)) / dz
	        	                -
	        	                (Hz(it+0.5, ix+0.5, iy, iz) - Hz(it+0.5, ix-0.5, iy, iz)) / dx
	        	                - jy
	        	                );//TODO сравнить j и rot(H)

	        });

	        cout << "Ez" << endl;
	        Ez[it+1].iterateInternal(1,1,0,GRID3D_ITERATOR{
	            double EPS = avg(epsilon(0,ix,iy,iz-0.5),epsilon(0,ix,iy,iz+0.5));

	            double SIGMA=avg(sigma(0,ix,iy,iz-0.5),sigma(0,ix,iy,iz+0.5));

	            /*
	            Ez(it + 1, ix, iy, iz)=
	                (1.0-SIGMA*DT/2.0/EPS/EPS0)/(1.0+SIGMA*DT/2.0/EPS/EPS0)*Ez(it, ix, iy, iz)
	                + DT/(1.0+SIGMA*DT/2.0/EPS/EPS0)/EPS/EPS0 * (
	                (Hy(it+0.5, ix+0.5, iy, iz) - Hy(it+0.5, ix-0.5, iy, iz)) / dx
	                -
	                (Hx(it+0.5, ix, iy+0.5, iz) - Hx(it+0.5, ix, iy-0.5, iz)) / dy
	                );
	            */

	            const double n_e = avg( HdVec3D::density(U(it,ix-0.5,iy-0.5,iz)) , HdVec3D::density(U(it,ix-0.5,iy+0.5,iz)) ,HdVec3D::density(U(it,ix+0.5,iy-0.5,iz)) ,HdVec3D::density(U(it,ix+0.5,iy+0.5,iz))); // [1/m^3]
	            const double V_z = avg( HdVec3D::velocityZ(U(it,ix-0.5,iy-0.5,iz)) , HdVec3D::velocityZ(U(it,ix-0.5,iy+0.5,iz)) ,HdVec3D::velocityZ(U(it,ix+0.5,iy-0.5,iz)) ,HdVec3D::velocityZ(U(it,ix+0.5,iy+0.5,iz)) );

	            const double jz = cze * V_z * n_e;// [A/м^2]


	            Ez(it + 1, ix, iy, iz) =
	            		DT/EPS0/EPS * (
	        	                (Hy(it+0.5, ix+0.5, iy, iz) - Hy(it+0.5, ix-0.5, iy, iz)) / dx
	        	                -
	        	                (Hx(it+0.5, ix, iy+0.5, iz) - Hx(it+0.5, ix, iy-0.5, iz)) / dy
	        	                -jz
	        	                );//TODO сравнить j и rot(H)

	        });


	        /*Liao 3rd order absorbing boundary conditions



	         */
	        //cout << "Ex1" << endl;
	        Ex[it+1].iterateBorderMinY(GRID3D_ITERATOR{
	            double u1=Ex(it,ix,iy+1,iz);
	            double u2=Ex(it-1,ix,iy+2,iz);
	            double u3=Ex(it-2,ix,iy+3,iz);
	            double u4=Ex(it-3,ix,iy+4,iz);

	            Ex(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });
	        //cout << "Ex2" << endl;
	        Ex[it+1].iterateBorderMaxY(GRID3D_ITERATOR{
	            double u1=Ex(it,ix,iy-1,iz);
	            double u2=Ex(it-1,ix,iy-2,iz);
	            double u3=Ex(it-2,ix,iy-3,iz);
	            double u4=Ex(it-3,ix,iy-4,iz);

	            Ex(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });
	        //cout << "Ex3" << endl;
	        Ex[it+1].iterateBorderMinZ(GRID3D_ITERATOR{
	            double u1=Ex(it,ix,iy,iz+1);
	            double u2=Ex(it-1,ix,iy,iz+2);
	            double u3=Ex(it-2,ix,iy,iz+3);
	            double u4=Ex(it-3,ix,iy,iz+4);

	            Ex(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });
	        //cout << "Ex4" << endl;
	        Ex[it+1].iterateBorderMaxZ(GRID3D_ITERATOR{
	            double u1=Ex(it,ix,iy,iz-1);
	            double u2=Ex(it-1,ix,iy,iz-2);
	            double u3=Ex(it-2,ix,iy,iz-3);
	            double u4=Ex(it-3,ix,iy,iz-4);

	            Ex(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });


	        //cout << "Ex5" << endl;
	        Ey[it+1].iterateBorderMinX(GRID3D_ITERATOR{
	            double u1=Ey(it,ix+1,iy,iz);
	            double u2=Ey(it-1,ix+2,iy,iz);
	            double u3=Ey(it-2,ix+3,iy,iz);
	            double u4=Ey(it-3,ix+4,iy,iz);

	            Ey(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });

	        //cout << "Ex6" << endl;
	        Ey[it+1].iterateBorderMaxX(GRID3D_ITERATOR{
	            double u1=Ey(it,ix-1,iy,iz);
	            double u2=Ey(it-1,ix-2,iy,iz);
	            double u3=Ey(it-2,ix-3,iy,iz);
	            double u4=Ey(it-3,ix-4,iy,iz);

	            Ey(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });

	        //cout << "Ex7" << endl;
	        Ey[it+1].iterateBorderMinZ(GRID3D_ITERATOR{
	            double u1=Ey(it,ix,iy,iz+1);
	            double u2=Ey(it-1,ix,iy,iz+2);
	            double u3=Ey(it-2,ix,iy,iz+3);
	            double u4=Ey(it-3,ix,iy,iz+4);

	            Ey(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });

	        //cout << "Ex8" << endl;
	        Ey[it+1].iterateBorderMaxZ(GRID3D_ITERATOR{
	            double u1=Ey(it,ix,iy,iz-1);
	            double u2=Ey(it-1,ix,iy,iz-2);
	            double u3=Ey(it-2,ix,iy,iz-3);
	            double u4=Ey(it-3,ix,iy,iz-4);

	            Ey(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });


	        //cout << "Ex9" << endl;
	        Ez[it+1].iterateBorderMinX(GRID3D_ITERATOR{
	            double u1=Ez(it,ix+1,iy,iz);
	            double u2=Ez(it-1,ix+2,iy,iz);
	            double u3=Ez(it-2,ix+3,iy,iz);
	            double u4=Ez(it-3,ix+4,iy,iz);

	            Ez(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });
	        //cout << "Ex10" << endl;
	        Ez[it+1].iterateBorderMaxX(GRID3D_ITERATOR{
	            double u1=Ez(it,ix-1,iy,iz);
	            double u2=Ez(it-1,ix-2,iy,iz);
	            double u3=Ez(it-2,ix-3,iy,iz);
	            double u4=Ez(it-3,ix-4,iy,iz);

	            Ez(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });


	        //cout << "Ex11" << endl;
	        Ez[it+1].iterateBorderMinY(GRID3D_ITERATOR{
	            double u1=Ez(it,ix,iy+1,iz);
	            double u2=Ez(it-1,ix,iy+2,iz);
	            double u3=Ez(it-2,ix,iy+3,iz);
	            double u4=Ez(it-3,ix,iy+4,iz);

	            Ez(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });
	        //cout << "Ex12" << endl;
	        Ez[it+1].iterateBorderMaxY(GRID3D_ITERATOR{
	            double u1=Ez(it,ix,iy-1,iz);
	            double u2=Ez(it-1,ix,iy-2,iz);
	            double u3=Ez(it-2,ix,iy-3,iz);
	            double u4=Ez(it-3,ix,iy-4,iz);

	            Ez(it+1,ix,iy,iz)=interpLiao3(u1,u2,u3,u4);
	        });




	        cout << "Hx" << endl;
	        Hx[it+1.5].iterateWhole(GRID3D_ITERATOR{
	            double MU = avg(mu(0,ix-0.5,iy,iz),mu(0,ix+0.5,iy,iz));

	            Hx(it+1.5, ix, iy, iz)=
	                Hx(it+0.5, ix, iy, iz) -
	                DT/MU/MU0  * (
	                (Ez(it+1, ix, iy+0.5, iz) - Ez(it+1, ix, iy-0.5, iz)) / dy
	                -
	                (Ey(it+1, ix, iy, iz+0.5) - Ey(it+1, ix, iy, iz-0.5)) / dz
	                );
	        });

	        cout << "Hy" << endl;
	        Hy[it+1.5].iterateWhole(GRID3D_ITERATOR{
	            double MU = avg(mu(0,ix,iy-0.5,iz),mu(0,ix,iy+0.5,iz));

	            Hy(it+1.5, ix, iy, iz)=
	                Hy(it+0.5, ix, iy, iz) -
	                DT/MU/MU0  * (
	                (Ex(it+1, ix, iy, iz+0.5) - Ex(it+1, ix, iy, iz-0.5)) / dz
	                -
	                (Ez(it+1, ix+0.5, iy, iz) - Ez(it+1, ix-0.5, iy, iz)) / dx
	                );
	        });


	        cout << "Hz" << endl;
	        Hz[it+1.5].iterateWhole(GRID3D_ITERATOR{
	            double MU = avg(mu(0,ix,iy,iz-0.5),mu(0,ix,iy,iz+0.5));

	            Hz(it+1.5, ix, iy, iz)=
	                Hz(it+0.5, ix, iy, iz) -
	                DT/MU/MU0  * (
	                (Ey(it+1, ix+0.5, iy, iz) - Ey(it+1, ix-0.5, iy, iz)) / dx
	                -
	                (Ex(it+1, ix, iy+0.5, iz) - Ex(it+1, ix, iy-0.5, iz)) / dy
	                );
	        });


	     cout << "HD step" << endl;


	    cout << "HD step x" << endl;
	    Fx[it].iterateInternal(1, 0, 0, GRID3D_ITERATOR {
			Fx(it,ix,iy,iz)=HdFlowVec3D::X::riemannFlux( U(it,ix-0.5,iy,iz) , U(it,ix+0.5,iy,iz) );
//
//			DBGVAL( U(it,ix-0.5,iy,iz)[0] );
//			DBGVAL( U(it,ix-0.5,iy,iz)[1] );
//			DBGVAL( U(it,ix-0.5,iy,iz)[2] );
//			DBGVAL( U(it,ix-0.5,iy,iz)[3] );
//			DBGVAL( U(it,ix-0.5,iy,iz)[4] );
//
//			DBGVAL( U(it,ix+0.5,iy,iz)[0] );
//			DBGVAL( U(it,ix+0.5,iy,iz)[1] );
//			DBGVAL( U(it,ix+0.5,iy,iz)[2] );
//			DBGVAL( U(it,ix+0.5,iy,iz)[3] );
//			DBGVAL( U(it,ix+0.5,iy,iz)[4] );
//
//			DBGVAL( Fx(it,ix,iy,iz)[0] );
//			DBGVAL( Fx(it,ix,iy,iz)[1] );
//			DBGVAL( Fx(it,ix,iy,iz)[2] );
//			DBGVAL( Fx(it,ix,iy,iz)[3] );
//			DBGVAL( Fx(it,ix,iy,iz)[4] );

//			pressAnyKey();
		});

	    cout << "HD step y" << endl;
		Fy[it].iterateInternal(0, 1, 0, GRID3D_ITERATOR {
			Fy(it,ix,iy,iz)=HdFlowVec3D::Y::riemannFlux( U(it,ix,iy-0.5,iz) , U(it,ix,iy+0.5,iz) );
		});

		cout << "HD step z" << endl;
		Fz[it].iterateInternal(0, 0, 1, GRID3D_ITERATOR {
			Fz(it,ix,iy,iz)=HdFlowVec3D::Z::riemannFlux( U(it,ix,iy,iz-0.5) , U(it,ix,iy,iz+0.5) );
		});

		cout << "Hd boundary conditions" << endl;

		//TODO: change boundary conditions to more corect
		//TODO: where to place boundary conditions before or after calculation?
		//transparent BC
		Fx[it].iterateBorderMinX(GRID3D_ITERATOR {
			Fx(it,ix,iy,iz)=Fx(it,ix+1,iy,iz);
		});

		Fx[it].iterateBorderMaxX(GRID3D_ITERATOR {
			Fx(it,ix,iy,iz)=Fx(it,ix-1,iy,iz);
		});

		Fy[it].iterateBorderMinY(GRID3D_ITERATOR {
			Fy(it,ix,iy,iz)=Fy(it,ix,iy+1,iz);
		});

		Fy[it].iterateBorderMaxY(GRID3D_ITERATOR {
			Fy(it,ix,iy,iz)=Fy(it,ix,iy-1,iz);
		});

		Fz[it].iterateBorderMinZ(GRID3D_ITERATOR {
			Fz(it,ix,iy,iz)=Fz(it,ix,iy,iz+1);
		});

		Fz[it].iterateBorderMaxZ(GRID3D_ITERATOR {
			Fz(it,ix,iy,iz)=Fz(it,ix,iy,iz-1);
		});


		U[it + 1].iterateWhole(GRID3D_ITERATOR {
			U(it+1,ix,iy,iz)=U(it,ix,iy,iz) - DT/dx*(Fx(it,ix+0.5,iy,iz)-Fx(it,ix-0.5,iy,iz))
			                                - DT/dy*(Fy(it,ix,iy+0.5,iz)-Fy(it,ix,iy-0.5,iz))
			                                - DT/dz*(Fz(it,ix,iy,iz+0.5)-Fz(it,ix,iy,iz-0.5));

//			DBGVAL( Fx(it,ix+0.5,iy,iz)[0] );
//			DBGVAL( Fx(it,ix+0.5,iy,iz)[1] );
//			DBGVAL( Fx(it,ix+0.5,iy,iz)[2] );
//			DBGVAL( Fx(it,ix+0.5,iy,iz)[3] );
//			DBGVAL( Fx(it,ix+0.5,iy,iz)[4] );

			//DISSIPATION STEP

			const double n_e = HdVec3D::density(U(it,ix,iy,iz)) / cme;// концентрация электронов
			const double WEt = HdVec3D::internalEnergyPerVolumeUnit(U(it,ix,iy,iz)) / n_e / Wevlt;//электронная температура [эВ]

			int N = 5;//размерность системы уравнений
			const int USER_DATA_N = ceil(1.*sizeof(UserData)/sizeof(double));//размер данных, передаваемых в функцию решения диф. уравнения (в размерах double)
			double y[N + USER_DATA_N];

			const double vx = HdVec3D::velocityX( U(it+1,ix,iy,iz) );
			const double vy = HdVec3D::velocityY( U(it+1,ix,iy,iz) );
			const double vz = HdVec3D::velocityZ( U(it+1,ix,iy,iz) );

			const double E_internal = HdVec3D::internalEnergyPerVolumeUnit( U(it+1,ix,iy,iz) )/n_e;//Внутренняя энергия на одлин электрон [Дж]

//			DBGLN("before dissipation");
//
//			DBGVAL( HdVec3D::fullEnergyPerVolumeUnit( U(it,ix,iy,iz) ) );
//			DBGVAL( HdVec3D::kineticEnergyPerVolumeUnit( U(it,ix,iy,iz) ) );
//			DBGVAL( HdVec3D::internalEnergyPerVolumeUnit( U(it,ix,iy,iz) )  );
//
//			DBGVAL( HdVec3D::fullEnergyPerVolumeUnit( U(it+1,ix,iy,iz) ) );
//			DBGVAL( HdVec3D::kineticEnergyPerVolumeUnit( U(it+1,ix,iy,iz) ) );
//			DBGVAL( HdVec3D::internalEnergyPerVolumeUnit( U(it+1,ix,iy,iz) )  );


			y[0] = HdVec3D::density( U(it+1,ix,iy,iz) ) / cme;//концентрация электронов
			y[1] = vx;
 			y[2] = vy;
			y[3] = vz;
			y[4] = E_internal;//TODO

			UserData* userdata = (UserData*)(&y[N]);
			userdata->E_x = avg( Ex(it+1,ix,iy-0.5,iz-0.5) , Ex(it+1,ix,iy-0.5,iz+0.5) , Ex(it+1,ix,iy+0.5,iz-0.5) , Ex(it+1,ix,iy+0.5,iz+0.5) ); //TODO может сделать усреднение и по слоям времени?
			userdata->E_y = avg( Ey(it+1,ix-0.5,iy,iz-0.5) , Ey(it+1,ix-0.5,iy,iz+0.5) , Ey(it+1,ix+0.5,iy,iz-0.5) , Ey(it+1,ix+0.5,iy,iz+0.5) ); //TODO может сделать усреднение и по слоям времени?
			userdata->E_z = avg( Ez(it+1,ix-0.5,iy-0.5,iz) , Ez(it+1,ix-0.5,iy+0.5,iz) , Ez(it+1,ix+0.5,iy-0.5,iz) , Ez(it+1,ix+0.5,iy+0.5,iz) ); //TODO может сделать усреднение и по слоям времени?

			userdata->H_x = avg( Hx(it+0.5,ix-0.5,iy,iz) , Hx(it+0.5,ix+0.5,iy,iz) , Hx(it+1.5,ix-0.5,iy,iz) , Hx(it+1.5,ix+0.5,iy,iz) );//TODO нужно ди усреднение по времени?
			userdata->H_y = avg( Hy(it+0.5,ix,iy-0.5,iz) , Hy(it+0.5,ix,iy+0.5,iz) , Hy(it+1.5,ix,iy-0.5,iz) , Hy(it+1.5,ix,iy+0.5,iz) );//TODO нужно ди усреднение по времени?
			userdata->H_z = avg( Hz(it+0.5,ix,iy,iz-0.5) , Hz(it+0.5,ix,iy,iz+0.5) , Hz(it+1.5,ix,iy,iz-0.5) , Hz(it+1.5,ix,iy,iz+0.5) );//TODO нужно ди усреднение по времени?

			userdata->V_SQR = sqr(vx,vy,vz);

			double time=0;
			double time_end=DT;

			double hm=DT * 1e-2; /* minimal step size for the methods */
			double ep=1e-6;  /* relative tolerance. The code cannot guarantee the requested accuracy for ep<1.d-9 */
			double tr=1e-50;  /* absolute tolerance */
		    double h=hm;

		    const int DPAR_SIZE = max(13*N,(7+2*N)*N);	//As ODE system has size n=2, than the size of dpar array is equal to
		   	                                            //max{13*n,(7+2*n)*n}=max{26,22}=26. More details on dpar array can be
		   	                                            //found in the Manual
	    	double dpar[DPAR_SIZE];
	    	int kd[N];

	    	int ierr=0;

			//dodesol(ipar,&N,&time,&time_end,y,rhs,NULL,&h,&hm,&ep,&tr,dpar,kd,&ierr);


	    	double f[N];
	    	rhs(&N,NULL,y,f);

	    	y[0] += f[0]*DT;
	    	y[1] += f[1]*DT;
	    	y[2] += f[2]*DT;
	    	y[3] += f[3]*DT;
	    	y[4] += f[4]*DT;

			double rho=y[0]*cme;
			double wx=y[1];
			double wy=y[2];
			double wz=y[3];
			double TE=y[4] * y[0];

			U(it+1,ix,iy,iz) = HdVec3D::fromDesityVelocityInternalEnergyPerVolumeUnit(rho,wx,wy,wz,TE);


//			DBGLN("\n\nafter dissipation");
//
//		    DBGVAL( HdVec3D::fullEnergyPerVolumeUnit( U(it,ix,iy,iz) ) );
//			DBGVAL( HdVec3D::kineticEnergyPerVolumeUnit( U(it,ix,iy,iz) ) );
//			DBGVAL( HdVec3D::internalEnergyPerVolumeUnit( U(it,ix,iy,iz) )  );
//
//			DBGVAL( HdVec3D::fullEnergyPerVolumeUnit( U(it+1,ix,iy,iz) ) );
//			DBGVAL( HdVec3D::kineticEnergyPerVolumeUnit( U(it+1,ix,iy,iz) ) );
//			DBGVAL( HdVec3D::internalEnergyPerVolumeUnit( U(it+1,ix,iy,iz) )  );

		});

//		DBGLN("U0");
//		printStats(acc_U0);
//		DBGLN("U1");
//		printStats(acc_U1);
//		DBGLN("U2");
//		printStats(acc_U2);
//		DBGLN("U3");
//		printStats(acc_U3);
//		DBGLN("U4");
//		printStats(acc_U4);


		std::cout <<"\n\n" <<std::endl;


//		DBGLN("f0");
//		printStats(acc_f0);
//		DBGLN("f1");
//		printStats(acc_f1);
//		DBGLN("f2");
//		printStats(acc_f2);
//		DBGLN("f3");
//		printStats(acc_f3);
//		DBGLN("f4");
//		printStats(acc_f4);

		//pressAnyKey();


	    if (isEveryNth(it, 10)) {

	    	cimgSaver.saveSliceZ(Hz[it + 0.5], Nz / 2, frame("Hz_", it, "png"),GRID3D_CALCULATOR{
	    		return Hz(it + 0.5,ix,iy,iz);
	    	});

	    	/*
	    	cimgSaver.saveSliceZ(Hz[it + 0.5], Nz / 2, frame("Hz_", it, "png"),GRID3D_CALCULATOR{
	    		return Hz(it + 0.5,ix,iy,iz);
	    	});

			cimgSaver.saveSliceZ(Hz[it + 0.5], Nz / 2, frame("Hz_", it, "png"),GRID3D_CALCULATOR{
				return Hz(it + 0.5,ix,iy,iz);
			});
			*/
		}

	    if (it<20 || isEveryNth(it, 10)) {

	    	vtiSaver.save(Hx[it+0.5],frame("Hx_",it,"vti"));
	    	vtiSaver.save(Hy[it+0.5],frame("Hy_",it,"vti"));
			vtiSaver.save(Hz[it+0.5],frame("Hz_",it,"vti"));

	    	vtiSaver.save(Ex[it+0.5],frame("Ex_",it,"vti"));
	    	vtiSaver.save(Ey[it+0.5],frame("Ey_",it,"vti"));
			vtiSaver.save(Ez[it+0.5],frame("Ez_",it,"vti"));

			vtiSaver.save(U[it],frame("n_",it,"vti"),GRID3D_CALCULATOR{
				return U(it,ix,iy,iz)[0];
			});

			vtiSaver.save(U[it],frame("E_",it,"vti"),GRID3D_CALCULATOR{
				return U(it,ix,iy,iz)[4];
			});

		}


		}
		mainTimer.logTime("calculation finished");



	return 0;
}


void rhs(int *ptrN,double *t,double *u,double *f){
	int N = *ptrN;

	double n_e = u[0];

	double V_x = u[1];
	double V_y = u[2];
	double V_z = u[3];

	double Et = u[4];//тепловая энергия одного электрона [Дж]
	double WEt = J_TO_EV(Et);//тепловая энергия одного электрона [эВ]

//	DBGVAL(WEt);

	const double n_i = n_e; //концентрация положительных ионов [м^-3] TODO why n_i ~= n_e ?

    const double n_all = ATMOSPHERE_DENSITY_40KM / AVERAGE_AIR_ION_MASS;
	const double n_O2 = ATMOSPHERE_O2_CONCENTRATION_40KM;
	const double n = n_all - n_O2; //концентрация нейтралов + положительных ионов TODO[] отсылка к Ступицкому
	const double n_0 = n - n_i;//т.к n = n_i + n_0 (такие обозначения)


	const UserData* userdata = (UserData*)(&u[N]);

    const double E_x = userdata->E_x;
    const double E_y = userdata->E_y;
    const double E_z = userdata->E_z;

    const double H_x = userdata->H_x;
    const double H_y = userdata->H_y;
    const double H_z = userdata->H_z;

    const double V_SQR = userdata->V_SQR;

    const double m_e = ELECTRON_MASS;
    const double m = m_e;
    const double M = AVERAGE_AIR_ION_MASS;
    const double e = ELECTRON_MASS;


    const double f_ = 11.67*(1.0+0.64*(WEt-11.35)/(88.65+WEt))*(1.0-exp(-0.0083*(WEt-11.35)));// [безразмерно]
    const double sgm =  WEt>ci  ?  f_*4.0*PI_A0_SQR* sqr(Ry/WEt)*(WEt-ci)/ci :  0;// [m^2] TODO: Порог (WEt должно быть > ci) Se=0 или sgm=0
    const double V = 5.3E7*sqrt(WEt)                          * 1E-2;//[m/s]
//    DBGVAL(f_);
//    DBGVAL(sgm);
//    DBGVAL(V);

    const double j_0e = sgm*V;//[m^3/s]
    const double j_g = 1.16E-8 / WEt                          *1E-6;// [m^3/s]
    const double j_v = 2.7E-13/pow(2.0/3.0*WEt,3.0/4.0)       *1E-6;// [m^3/s]

    const double j_ei = 8.75E-27/ pow((2.0/3.0*WEt),9.0/2.0)  *1E-12;// [m^6/s]
    const double j_p = 3.8E-31/WEt * exp(-0.103/WEt)          *1E-12;// [m^6/s]

    const double kT = 2.0/3.0*WEt;//температура электронов в эВ [эВ]


    const double L = 25.2 + log(2.0*WEt/3.0/ckb)- 0.5*log(n_e        * 1E-6);//TODO check for range 15..18; decimal or natural logorythm
//    DBGVAL(L);
    const double Z =1.0;

    const double sigma_e0 = 12.47 * PI_A0_SQR * (0.4 + 0.84*WEt/(0.5 + WEt));// [м^2]

    const double v_ei = 16.0*sqrt(M_PI)/3.0 * quad( COULUMB_TO_CGS(cze) ) * L * n_i * sqr(Z)/sqrt(m_e)/pow(2.0*kT,3.0/2.0);
    const double v_e0 = 8.0*sigma_e0/3.0/sqrt(M_PI)*sqrt(2.0* EV_TO_J(kT)/m_e)*n_0;


    const double S_e = WEt>ci  ?  (j_0e*n_e*n_0 - j_ei*sqr(n_e)*n_i) - j_v*n_e*n_i - j_g*n_e*n_i - j_p*n_e*n_O2*n   :  0.0;//TODO [?] пороги!!!

    const double fi = 0.64 + 0.11*log(I/kT);//TODO natural or decimal
    const double S_ee = -(I+3.0/2.0*kT)*Wevlt*(n_e*n_0*j_0e - sqr(n_e)*n_i*j_ei)+
    		            + (3.0/2.0-fi)*kT*Wevlt*n_e*n_i*j_v - 3.0/2.0*kT*Wevlt*j_g*n_e*n_i - 3.0/2.0*kT*Wevlt*j_p*n_e*n_O2*n;//TODO заменить Wevlt на EV_TO_J()

    const double WEt_ = 0.025;//TODO что это? сколько это градусов?
    const double Q_ei = -2.0 * v_ei*n_e*(WEt-WEt_)*Wevlt*m_e/M + m*V_SQR/2.0;//TODO заменить Wevlt на EV_TO_J() ? порог??? WEt должно быть > WEt_

    const double nu = 6E-8 * n_0*sqrt(WEt)*(0.4 + 0.84*WEt/(0.5+WEt))      *1E-6;// [s^-1]   1E6-коэффициент перевода для концентрации из СИ в СГС (так как оригинальная формула написана в СГС)
    const double delta = 1.7E-3 * (1.0 + 0.2*pow(WEt/0.9,5)) / (1.0 + 3.7E-2*(1.0 + 0.2*(pow(WEt/0.9,5))) );// безразмерно

    const double Q_e0 = -n_e*EV_TO_J(WEt-WEt_)*nu*delta;//TODO см диплом  порог

    const double Q_e = Q_ei + Q_e0;
    //const double Q_w = e*n_e*(abs(E_x*V_x) + abs(E_y*V_y) + abs(E_z*V_z));//TODO не симметрично относительно поворота СК, что имел ввиду Ступитский?
    //const double Q_w = e*n_e*(E_x*V_x + E_y*V_y + E_z*V_z);//TODO не симметрично относительно поворота СК, размерность нормальная? [Дж/с/м^3]
    const double Q_w = e*n_e*sqrt( sqr(E_x*V_x , E_y*V_y , E_z*V_z) );


	f[0] = S_e;
    f[1] = -(v_ei+v_e0)*V_x  -  e*E_x/m  +  e*MU*MU0/m*V_z*H_y - e*MU*MU0/m*V_y*H_z;
    f[2] = -(v_ei+v_e0)*V_y  -  e*E_y/m  +  e*MU*MU0/m*V_x*H_z - e*MU*MU0/m*V_z*H_x;
    f[3] = -(v_ei+v_e0)*V_z  -  e*E_z/m  +  e*MU*MU0/m*V_y*H_x - e*MU*MU0/m*V_x*H_y;
    f[4] = (S_ee + Q_e + Q_w)/n_e;

    #if(0)
    if((isnan(u[0])==0) && (isinf(u[0])==0)) acc_U0(u[0]);
    if((isnan(u[1])==0) && (isinf(u[1])==0)) acc_U1(u[1]);
    if((isnan(u[2])==0) && (isinf(u[2])==0)) acc_U2(u[2]);
    if((isnan(u[3])==0) && (isinf(u[3])==0)) acc_U3(u[3]);
    if((isnan(u[4])==0) && (isinf(u[4])==0)) acc_U4(u[4]);

    if((isnan(f[0])==0) && (isinf(f[0])==0)) acc_f0(f[0]);
    if((isnan(f[1])==0) && (isinf(f[1])==0)) acc_f1(f[1]);
    if((isnan(f[2])==0) && (isinf(f[2])==0)) acc_f2(f[2]);
    if((isnan(f[3])==0) && (isinf(f[3])==0)) acc_f3(f[3]);
    if((isnan(f[4])==0) && (isinf(f[4])==0)) acc_f4(f[4]);
    #endif

    #if(1)
//    DBGVAL(dt / *t);

//    DBGVAL(j_0e );
//    DBGVAL(n_e);
//    DBGVAL(n_0);
//
//    DBGVAL(- j_ei*sqr(n_e)*n_i);
//    DBGVAL(j_0e*n_e*n_0);
//    DBGVAL(- j_v*n_e*n_i);
//    DBGVAL(- j_g*n_e*n_i);
//    DBGVAL(- j_p*n_e*n_O2*n);
//

//    DBGVAL(v_ei);
//    DBGVAL(v_e0);
//    DBGVAL(-(v_ei+v_e0)*V_x);
//    DBGVAL(-  e*E_x/m);
//    DBGVAL(e*MU*MU0/m*V_z*H_y);
//    DBGVAL(- e*MU*MU0/m*V_z*H_x);











//    DBGVAL(-(I+3.0/2.0*kT)*Wevlt*(n_e*n_0*j_0e  ));
//    DBGVAL(-(I+3.0/2.0*kT)*Wevlt*(             - sqr(n_e)*n_i*j_ei));
//    DBGVAL((3.0/2.0-fi)*kT*Wevlt*n_e*n_i*j_v);
//    DBGVAL(-3.0/2.0*kT*Wevlt*j_g*n_e*n_i);
//    DBGVAL(-3.0/2.0*kT*Wevlt*j_p*n_e*n_O2*n);//TODO заменить Wevlt на EV_TO_J()
//
//
//    DBGVAL(S_ee/n_e);
//    DBGVAL(Q_e/n_e);
//    DBGVAL(Q_w/n_e);
//
//    DBGVAL(Q_ei/n_e);
//    DBGVAL(Q_e0/n_e);
//
//    DBGVAL(u[0]);
//    DBGVAL(u[1]);
//    DBGVAL(u[2]);
//    DBGVAL(u[3]);
//    DBGVAL(u[4]);
//
//    DBGVAL(f[0]);
//    DBGVAL(f[1]);
//    DBGVAL(f[2]);
//    DBGVAL(f[3]);
//    DBGVAL(f[4]);
//
//    DBGVAL(f[0]*dt);
//    DBGVAL(f[1]*dt);
//    DBGVAL(f[2]*dt);
//    DBGVAL(f[3]*dt);
//    DBGVAL(f[4]*dt);
//
//    DBGVAL(dt);
//
//    pressAnyKey();





//    DBGVAL(-(v_ei+v_e0)*V_x);
//    DBGVAL(-e*E_x/m);
//    DBGVAL(e*MU*MU0/m*V_z*H_y);
//    DBGVAL(- e*MU*MU0/m*V_y*H_z);
//
//
//    DBGVAL("  -----  ");
//
//    DBGVAL(-(v_ei+v_e0)*V_x);
//    DBGVAL(-e*E_x/m);
//    DBGVAL(e*MU*MU0/m*V_z*H_y);
//    DBGVAL(- e*MU*MU0/m*V_y*H_z);
//
//    DBGVAL("  -----  ");
//
//    DBGVAL(-(v_ei+v_e0)*V_y);
//    DBGVAL(-  e*E_y/m);
//    DBGVAL(e*MU*MU0/m*V_x*H_z);
//    DBGVAL(- e*MU*MU0/m*V_z*H_x);
//
//    DBGVAL("  -----  ");
//
//    DBGVAL(-(v_ei+v_e0)*V_z);
//    DBGVAL(-  e*E_z/m);
//    DBGVAL(e*MU*MU0/m*V_y*H_x);
//    DBGVAL(- e*MU*MU0/m*V_x*H_y);


    #endif


}
