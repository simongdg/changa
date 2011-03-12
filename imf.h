#ifndef IMF_HINCLUDED
#define IMF_HINCLUDED

// Please see 
// http://charm.cs.uiuc.edu/manuals/html/charm++/3_17.html#SECTION000317400000000000000
// for details about PUPing child and parent classes.

// http://www.parashift.com/c++-faq-lite/abcs.html#faq-22.5
// for copy constructors in inherited abstract classes and a
// description of pure virtual functions

class IMF : public PUP::able {

 public:
    double a1, b1, m1;
    double a2, b2, m2;
    double a3, b3, m3;
    double mmax;

    double returnimf(double mass);
    //    double imfIntM(double logMass, void * params);
 IMF(): a1(0),b1(0),m1(0), a2(0), b2(0), m2(0), a3(0), b3(0), m3(0),mmax(0) {}
    IMF(double _a1,double _b1,double _m1,double _a2, double _b2, double _m2,
	double _a3, double _b3, double _m3, double _mmax) :a1(_a1),b1(_b1),
      m1(_m1), a2(_a2), b2(_b2), m2(_m2), a3(_a3), b3(_b3), m3(_m3),mmax(_mmax){}
    PUPable_abstract(IMF);
    virtual void pup(PUP::er &p) {
	PUP::able::pup(p);
	p|a1; p|b1; p|m1;
	p|a2; p|b2; p|m2;
	p|a3; p|b3; p|m3;
	p|mmax;
	}
 IMF(CkMigrateMessage *m) : PUP::able(m) {}
    virtual double CumNumber(double mass);
    virtual double CumMass(double mass);
    virtual IMF* clone() const = 0;
    double Oneto8Exp();
    double Oneto8PreFactor();
    virtual ~IMF() {}
};


/*
    Uses the 3 segment power law fit for the Miller-Scalo IMF
    (Ap.J. Supp., 41,1979).

                                a1*(M**(b1))          0.1<M<1.
              IMF(Log10(M))=    a2*(M**(b2))         1.<M<10.
                                a3*(M**(b3))         10.<M
   Miller-Scalo IMF (Miller & Scalo, Ap.J. Supp., 41, 513, 1979) in
   stars per unit logarithmic mass.  Divide by M (mass) for IMF in
   stars per unit mass.  Also IMF is defined per yer per pc^2,
   integrated over a cylinder that extends "several hundred parsecs on
   either side of the plane of the galaxy"
*/

class MillerScalo : public IMF {
 public:
    /* normalization, index, minimum mass */
    MillerScalo() {
	a1=42.0; b1=-0.4;m1=0.1; /* parameters from Ap.J. Supp., 41,1979 */
	a2=42.0;b2=-1.5;m2=1.0; /* This is discontinuous, but is what */
	a3=240.0;b3=-2.3;m3=10.0;/* they report in paper, so we leave it.*/
	mmax=100.0;
	} 
    MillerScalo(const MillerScalo& ms) {
	a1 = ms.a1; b1 = ms.b1; m1 = ms.m1; 
	a2 = ms.a2; b2 = ms.b2; m2 = ms.m2; 
	a3 = ms.a3; b3 = ms.b3; m3 = ms.m3; 
	mmax = ms.mmax;
	}
    PUPable_decl(MillerScalo);
    MillerScalo(CkMigrateMessage *m) : IMF(m) {}
    virtual MillerScalo* clone() const;
    virtual void pup(PUP::er &p) {
        IMF::pup(p);//Call base class
	}
    virtual ~MillerScalo() {}
    };

class Kroupa93 : public IMF {
 public:
/* parameters from Raiteri et. al. A&A, 315,1996, eq. 2;  See also the
   conclusions of Kroupa, Tout & Gilmore, 1993. */
    Kroupa93() {a1=0.3029*1.86606;b1=-0.3;m1=.08; 
	a2=0.3029;b2=-1.2;m2=0.5; 
	a3=0.3029; b3=-1.7; m3=1.0; 
	mmax=100.0; }
    Kroupa93(const Kroupa93& kroup) {
	a1 = kroup.a1; b1 = kroup.b1; m1 = kroup.m1; 
	a2 = kroup.a2; b2 = kroup.b2; m2 = kroup.m2; 
	a3 = kroup.a3; b3 = kroup.b3; m3 = kroup.m3; 
	mmax = kroup.mmax;
	}
    PUPable_decl(Kroupa93);
    Kroupa93(CkMigrateMessage *m) : IMF(m) {}
    virtual Kroupa93* clone() const;
    virtual void pup(PUP::er &p) {
        IMF::pup(p);//Call base class
	}
    virtual ~Kroupa93() {}
};

/*
  Use the log normal + power law fit of Chabrier, 2003, Galactic
  Stellar and Substellar Initial Mass Function", PASP 115, 763.
*/

class Chabrier : public IMF {
    double imf(double mass);
 public:
    /*
      Chabrier low mass formula:
      \xi(log m) = A exp [ - (log m - log m_c)^2/2 \sigma^2]
      double a1, sigma, mc;  (b1 is sigma, m1 is mc)
    */
    Chabrier() {
	a1=0.158;b1=0.69;m1=.079;
	/* For high mass: normalization, index, minimum mass */
	/* Parameters from Table 1 of Chabrier, 2003. */
	a2=4.43e-2;b2=-1.3; m2=1.0;
	a3=4.43e-2;b3=-1.3; m3=1.0;
	mmax=100.0;
	}
    Chabrier(const Chabrier& chab) {
	a1 = chab.a1; b1 = chab.b1; m1 = chab.m1; 
	a2 = chab.a2; b2 = chab.b2; m2 = chab.m2; 
	a3 = chab.a3; b3 = chab.b3; m3 = chab.m3; 
	mmax = chab.mmax;
	}
    virtual double CumNumber(double mass);
    virtual double CumMass(double mass);
    virtual Chabrier* clone() const;
    PUPable_decl(Chabrier);
 Chabrier(CkMigrateMessage *m) : IMF(m) {}
    virtual void pup(PUP::er &p) {
        IMF::pup(p);//Call base class
	}
    virtual ~Chabrier() {};
};



#endif
