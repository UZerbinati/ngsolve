#ifndef FILE_HCURLLOFE
#define FILE_HCURLLOFE

/*********************************************************************/
/* File:   hcurllofe.hpp                                             */
/* Author: Joachim Schoeberl                                         */
/* Date:   16. Apr. 2000                                             */
/*********************************************************************/

#include "h1lofe.hpp"

namespace ngfem
{


  /* **************************** Segm Elements *************** */


  ///
  class FE_NedelecSegm1 : public HCurlFiniteElement<1>
  {
  public:

    enum { NDOF = 1 };

  private:
    ///
    // static Array<IPData> ipdata;
    // bool ipdatadestructed;

  public:
    ///
    FE_NedelecSegm1();
    ///
    virtual ~FE_NedelecSegm1();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<1> shape) const;
  };



  ///
  class FE_NedelecSegm2 : public HCurlFiniteElement<1>
  {
  public:

    enum { NDOF = 2 };

  private:
    ///
    // static Array<IPData> ipdata;
    // bool ipdatadestructed;

  public:
    ///
    FE_NedelecSegm2();
    ///
    virtual ~FE_NedelecSegm2();

    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<1> shape) const;
  };



  ///
  class FE_NedelecSegm3 : public HCurlFiniteElement<1>
  {
    ///
// static Array<IPData> ipdata;
// bool ipdatadestructed;

  public:
  ///
    FE_NedelecSegm3();
    ///
    virtual ~FE_NedelecSegm3();

    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<1> shape) const;
  };






  /* *********************** Quad elements ******************* */

  class FE_NedelecQuad1 : public T_HCurlFiniteElement<FE_NedelecQuad1,ET_QUAD,4,1>
  {
  public:
    template<typename Tx, typename TFA>  
    static void T_CalcShape (Tx hx[2], TFA & shape) 
    {
      Tx x = hx[0], y = hx[1];
      
      AutoDiff<2> lami[4] = {(1-x)*(1-y),x*(1-y),x*y,(1-x)*y};  
      AutoDiff<2> sigma[4] = {(1-x)+(1-y),x+(1-y),x+y,(1-x)+y};  
      
      const EDGE * edges = ElementTopology::GetEdges (ET_QUAD);
      for (int i = 0; i < 4; i++)
        {
          int es = edges[i][0], ee = edges[i][1];
          
          AutoDiff<2> xi  = sigma[ee]-sigma[es];
          AutoDiff<2> lam_e = lami[ee]+lami[es];  
          
          shape[i] = uDv<2> (0.5 * lam_e, xi); 
        }
    }
  };

  /*
 /// Gradients of Q1
 class FE_NedelecQuad1 : public HCurlFiniteElement<2>
 {
 public:
 enum { NDOF = 4 };

 protected:
 ///
 // static Array<IPData> ipdata;
 // bool ipdatadestructed;
 public:
 ///
 FE_NedelecQuad1();
 ///
 virtual ~FE_NedelecQuad1();

 ///
 virtual void CalcShape (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<2> shape) const;

 };
  */


  /*
    template <int ORDER, int ZORDER>
    class FE_TNedelecQuadTraits
    {
    public:
    enum { NDOF = ORDER * (ZORDER+1) + (ORDER+1) * ZORDER };
    enum { NEDGEDOF = 2 * (ORDER + ZORDER) - 4 };
    };
  */

  template <int ORDER, int ZORDER>
  class FE_TNedelecQuad : public HCurlFiniteElement<2>
  {
  public:
    enum { NDOF = ORDER * (ZORDER+1) + (ORDER+1) * ZORDER };
    enum { NEDGEDOF = 2 * (ORDER + ZORDER) - 4 };

  protected:
    ///
    // static Array<IPData> ipdata;
    // bool ipdatadestructed;
    ///
    // static Mat<FE_TNedelecQuad<ORDER,ZORDER>::NDOF> trans;

    // static Mat<FE_TNedelecQuadTraits<ORDER,ZORDER>::NDOF> trans;
    // static Mat<FE_TNedelecQuadTraits<ORDER,ZORDER>::NEDGEDOF> trans2;

    static Matrix<> trans;
    static Matrix<> trans2;

    FE_NedelecQuad1 quad1;

  public:
    enum { MAXORDER = (ORDER > ZORDER) ? ORDER : ZORDER };

    ///
    FE_TNedelecQuad();
    ///
    virtual ~FE_TNedelecQuad();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<2> shape) const;
    ///
    virtual void CalcShape1 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<2> shape) const;
    ///
    virtual void CalcShape2 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<2> shape) const;
    ///
    void Orthogonalize();  
  };




  /* ******************** triangular elements *********************** */

  class FE_NedelecTrig1 : public T_HCurlFiniteElement<FE_NedelecTrig1,ET_TRIG,3,1>
  {
  public:
    template<typename Tx, typename TFA>  
    static void T_CalcShape (Tx hx[2], TFA & shape) 
    {
      Tx x = hx[0], y = hx[1];
      Tx lami[3] = { x, y, 1-x-y };
      
      const EDGE * edges = ElementTopology::GetEdges (ET_TRIG);
      for (int i = 0; i < 3; i++)
        shape[i] = uDv_minus_vDu<2> (lami[edges[i][0]], lami[edges[i][1]]);
    }
  };

  class FE_NedelecTrig2 : public T_HCurlFiniteElement<FE_NedelecTrig2,ET_TRIG,6,1>
  {
  public:
    template<typename Tx, typename TFA>  
    static void T_CalcShape (Tx hx[2], TFA & shape) 
    {
      Tx x = hx[0], y = hx[1];
      Tx lami[3] = { x, y, 1-x-y };
      
      const EDGE * edges = ElementTopology::GetEdges (ET_TRIG);
      for (int i = 0; i < 3; i++)
        {
          shape[i] = uDv_minus_vDu<2> (lami[edges[i][0]], lami[edges[i][1]]);
          shape[i+3] = Du<2> (lami[edges[i][0]]*lami[edges[i][1]]);
        }
    }
  };

  class FE_NedelecTrig3 : public T_HCurlFiniteElement<FE_NedelecTrig3,ET_TRIG,12,2>
  {
  public:
    template<typename Tx, typename TFA>  
    static void T_CalcShape (Tx hx[2], TFA & shape) 
    {
      Tx x = hx[0], y = hx[1];
      Tx lami[3] = { x, y, 1-x-y };
      
      const EDGE * edges = ElementTopology::GetEdges (ET_TRIG);
      for (int i = 0; i < 3; i++)
        {
          Tx lam1 = lami[edges[i][0]];
          Tx lam2 = lami[edges[i][1]];

          shape[i] = uDv_minus_vDu<2> (lam1, lam2);
          shape[i+3] = Du<2> (lam1*lam2);
          shape[i+6] = Du<2> (lam1*lam2*(lam1-lam2));
        }

      const FACE * faces = ElementTopology::GetFaces (ET_TRIG); 
      for (int k = 0; k < 3; k++)
        {
          int k1 = (k+1)%3, k2 = (k+2)%3;
          shape[9+k] = uDv_minus_vDu<2> (lami[faces[0][k]],
                                         lami[faces[0][k1]]*lami[faces[0][k2]]);
        }

    }
  };



  /*
 /// Lowest order Nedelec
 class FE_NedelecTrig1 : public HCurlFiniteElement<2>
 {
 ///
 // static Array<IPData> ipdata;
 // bool ipdatadestructed;

 public:

 ///
 FE_NedelecTrig1();
 ///
 virtual ~FE_NedelecTrig1();
 ///
 virtual void CalcShape (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<2> shape) const;
 };



 /// Nedelec type 2, order 1, gradients of P2
 class FE_NedelecTrig2 : public HCurlFiniteElement<2>
 {
 public:
 enum { NDOF = 6 };

 private:
 ///
 // static Array<IPData> ipdata;
 // bool ipdatadestructed;
 ///
 static Mat<NDOF> trans;

 public:
 ///
 FE_NedelecTrig2();
 ///
 virtual ~FE_NedelecTrig2();
 ///
 virtual void CalcShape (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<2> shape) const;

 ///
 virtual void CalcShape1 (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<2> shape) const;

 ///
 void Orthogonalize();
 };


 /// Nedelec type 2, order 2, gradients of P3
 class FE_NedelecTrig3 : public HCurlFiniteElement<2>
 {
 public:
 enum { NDOF = 12 };
 enum { NEDGEDOF = 6 };
 ///
 // static Array<IPData> ipdata;
 // bool ipdatadestructed;
 ///
 static Mat<NDOF> trans;
 ///
 static Mat<NEDGEDOF> trans2;
 ///
 FE_NedelecTrig2 trig1;
 public:
 ///
 FE_NedelecTrig3();
 ///
 virtual ~FE_NedelecTrig3();
 ///
 virtual void CalcShape (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<2> shape) const;

 ///
 virtual void CalcShape1 (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<2> shape) const;

 ///
 virtual void CalcShape2 (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<2> shape) const;
  
 ///
 void Orthogonalize();
 };
  */










  /* *********************** Tetrahedral elements ********************** */
  
  class FE_NedelecTet1 : public T_HCurlFiniteElement<FE_NedelecTet1,ET_TET,6,1>
  {
  public:
    template<typename Tx, typename TFA>  
    static void T_CalcShape (Tx x[3], TFA & shape) 
    {
      // Tx x = hx[0], y = hx[1], z = hx[2];
      // Tx lami[4] = { x, y, z, 1-x-y-z };
      Tx lami[4] = { x[0], x[1], x[2], 1-x[0]-x[1]-x[2] };      

      const EDGE * edges = ElementTopology::GetEdges (ET_TET);
      for (int i = 0; i < 6; i++)
        shape[i] = uDv_minus_vDu<3> (lami[edges[i][0]], lami[edges[i][1]]);
    }
  };

  class FE_NedelecTet2 : public T_HCurlFiniteElement<FE_NedelecTet2,ET_TET,12,1>
  {
  public:
    template<typename Tx, typename TFA>  
    static void T_CalcShape (Tx x[3], TFA & shape) 
    {
      // Tx x = hx[0], y = hx[1], z = hx[2];
      // Tx lami[4] = { x, y, z, 1-x-y-z };
      Tx lami[4] = { x[0], x[1], x[2], 1-x[0]-x[1]-x[2] };      
      
      const EDGE * edges = ElementTopology::GetEdges (ET_TET);
      for (int i = 0; i < 6; i++)
        {
          shape[i] = uDv_minus_vDu<3> (lami[edges[i][0]], lami[edges[i][1]]);
          shape[i+6] = Du<3> (lami[edges[i][0]]*lami[edges[i][1]]);
        }
    }
  };


  class FE_NedelecTet3 : public T_HCurlFiniteElement<FE_NedelecTet3,ET_TET,30,2>
  {
  public:
    template<typename Tx, typename TFA>  
    static void T_CalcShape (Tx x[3], TFA & shape)
    {
      Tx lami[4] = { x[0], x[1], x[2], 1-x[0]-x[1]-x[2] };      
      
      const EDGE * edges = ElementTopology::GetEdges (ET_TET);
      for (int i = 0; i < 6; i++)
        {
          Tx lam1 = lami[edges[i][0]];
          Tx lam2 = lami[edges[i][1]];
          shape[i] = uDv_minus_vDu<3> (lam1, lam2);
          shape[i+6] = Du<3> (lam1*lam2);
          shape[i+12] = Du<3> (lam1*lam2*(lam1-lam2));
        }

      const FACE * faces = ElementTopology::GetFaces (ET_TET); 
      for (int i = 0; i < 4; i++)
        for (int k = 0; k < 3; k++)
          {
            int k1 = (k+1)%3, k2 = (k+2)%3;
            shape[18+3*i+k] = uDv_minus_vDu<3> (lami[faces[i][k]],
                                                lami[faces[i][k1]]*lami[faces[i][k2]]);
          }
    }
  };


  /*
    class FE_NedelecTet1o : public HCurlFiniteElement<3>
    {
    public:
    enum { NDOF = 6 };

    private:
    ///
    // static Array<IPData> ipdata;
    // bool ipdatadestructed;

    public:

    ///
    FE_NedelecTet1o();
    ///
    virtual ~FE_NedelecTet1o();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> shape) const;

    virtual void CalcCurlShape (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> curlshape) const;
    };


    ///
    class FE_NedelecTet2o : public HCurlFiniteElement<3>
    {
    public:

    enum { NDOF = 12 };

    private:
    ///
    // static Array<IPData> ipdata;
    // bool ipdatadestructed;
    ///
    static Mat<NDOF> trans;

    public:
    ///
    FE_NedelecTet2o();
    ///
    virtual ~FE_NedelecTet2o();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> shape) const;


    ///
    virtual void CalcShape1 (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> shape) const;

    ///
    void Orthogonalize();
    };


    /// 2nd order Nedelec element of class II
    class FE_NedelecTet3 : public HCurlFiniteElement<3>
    {
    public:
    enum { NDOF = 30 };
    enum { NEDGEDOF = 12 };
    enum { NFACEDOF = 12 };

    protected:
    ///
    //     static Array<IPData> ipdata;
    // bool ipdatadestructed;
    ///
    static Mat<NDOF> trans;
    ///
    static Mat<NEDGEDOF> trans2;
    ///
    static Mat<NFACEDOF> trans3;

    FE_NedelecTet1 tet1;
    public:

    ///
    FE_NedelecTet3();
    ///
    virtual ~FE_NedelecTet3();


    virtual void CalcShape (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> shape) const;

    virtual void CalcCurlShape (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> curlshape) const;
  
    ///
    virtual void CalcShape1 (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcShape2 (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcShape3 (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcCurlShape3 (const IntegrationPoint & ip, 
    FlatMatrixFixWidth<3> shape) const;

    ///
    void Orthogonalize();
    };
  */


  /// 2nd order Nedelec element of class II, without gradient fields
  class FE_NedelecTet3NoGrad : public HCurlFiniteElement<3>
  {
  public:
    enum { NDOF = 18 };
    enum { NFACEDOF = 12 };

  protected:
    static Mat<NFACEDOF> trans3;

    FE_NedelecTet1 tet1;
  public:
    FE_NedelecTet3NoGrad();
    virtual ~FE_NedelecTet3NoGrad();


    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<3> shape) const;

    virtual void CalcCurlShape (const IntegrationPoint & ip, 
                                FlatMatrixFixWidth<3> curlshape) const;

    virtual void CalcShape3 (const IntegrationPoint & ip, 
                             FlatMatrixFixWidth<3> shape) const;

    virtual void CalcCurlShape3 (const IntegrationPoint & ip, 
                                 FlatMatrixFixWidth<3> shape) const;

    void Orthogonalize();
  };



  /* *********************** Hex elements ************************ */ 



  /// 
  class FE_NedelecHex1 : public HCurlFiniteElement<3> 
  {
    /// 
// static Array<IPData> ipdata; 
// bool ipdatadestructed;
  
  public: 
  ///
    FE_NedelecHex1(); 
    ///
    virtual ~FE_NedelecHex1(); 
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
                            FlatMatrixFixWidth<3> shape) const; 
  }; 


  /* *********************** Prism elements ********************** */

  class FE_NedelecPrism1 : public T_HCurlFiniteElement<FE_NedelecPrism1,ET_PRISM,9,1>
  {
  public:
    template<typename Tx, typename TFA>  
    static void T_CalcShape (Tx hx[3], TFA & shape) 
    {
      Tx x = hx[0], y = hx[1], z = hx[2];

      Tx lami[6] = { x, y, 1-x-y, x, y, 1-x-y };
      Tx muz[6]  = { 1-z, 1-z, 1-z, z, z, z };
       
      const EDGE * edges = ElementTopology::GetEdges (ET_PRISM);
  
      // horizontal edge shapes
      for (int i = 0; i < 6; i++)
        {
          int es = edges[i][0], ee = edges[i][1]; 
          shape[i] = wuDv_minus_wvDu<3> (lami[es], lami[ee], muz[ee]);
        }
      
      //Vertical Edge Shapes
      for (int i = 6; i < 9; i++)
        {
          int es = edges[i][0], ee = edges[i][1];
          shape[i] = wuDv_minus_wvDu<3> (muz[es], muz[ee], lami[ee]);
        }
    }
  };
  
  /*
 ///
 class FE_NedelecPrism1 : public HCurlFiniteElement<3>
 {
 ///
 // static Array<IPData> ipdata;
 // bool ipdatadestructed;

 public:

 ///
 FE_NedelecPrism1();
 ///
 virtual ~FE_NedelecPrism1();

 ///
 virtual void CalcShape (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<3> shape) const;
 };
  */

  /// \f$ \nabla Q (2,ZORDER) \f$
  template <int ZORDER>
  class FE_TNedelecPrism2 : public HCurlFiniteElement<3>
  {
    ///
// static Array<IPData> ipdata;
// bool ipdatadestructed;
///
static Matrix<> trans;
    ///
static Matrix<> trans2;
    ///
static Matrix<> trans3;

    FE_NedelecPrism1 prism1;

  public:
    enum { NDOF = 6 * (ZORDER+1) + 6 * ZORDER };
    enum { NEDGEDOF = 6 + 3 * (ZORDER-1) };
    enum { NFACEDOF = 9 * ZORDER - 6} ;
    enum { MAXORDER = (2 > ZORDER) ? 2 : ZORDER };

    ///
    FE_TNedelecPrism2();
    ///
    virtual ~FE_TNedelecPrism2();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcShape1 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcShape2 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcShape3 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    ///
    void Orthogonalize();

  };



  /// potential space for Nedelec IIb
  class FE_Trig3Pot : public ScalarFiniteElement<2>
  {
    ///
// static IPDataArray ipdata;
  public:
  ///
    FE_Trig3Pot();
    ///
    virtual ~FE_Trig3Pot();

    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatVector<> shape) const;
			  
  }; 




  /// \f$ \nabla Q (3,ZORDER) \f$
  template <int ZORDER>
  class FE_TNedelecPrism3 : public HCurlFiniteElement<3>
  {
    ///
// static Array<IPData> ipdata;
// bool ipdatadestructed;
///
static Matrix<> trans;
    ///
static Matrix<> trans2;
    ///
static Matrix<> trans_quad;
    ///
static Matrix<> trans_trig;

    FE_NedelecPrism1 prism1;
    FE_NedelecTrig3 trig3;
    FE_Trig2 h1trig2;
    FE_Trig3Pot h1trig3;
    FE_TSegmL2<ZORDER> segm;
  public:
    enum { NDOF = 12 * (ZORDER+1) + 10 * ZORDER };
    enum { NEDGEDOF = 12 + 3 * (ZORDER-1) };
    enum { NQUADFACEDOF = 3 * (5*ZORDER-3) };
    enum { NTRIGFACEDOF = 6 };
    enum { MAXORDER = (3 > ZORDER) ? 3 : ZORDER };
    enum { NINNERDOF = 3 * (ZORDER-1) + ZORDER };

    ///
    FE_TNedelecPrism3();
    ///
    virtual ~FE_TNedelecPrism3();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcShape1 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcShape2 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    /// quad face dofs
    virtual void CalcShape3 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    /// trig face dofs
    virtual void CalcShape4 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcInner (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void GetInternalDofs (Array<int> & idofs) const;

    ///
    void Orthogonalize();
  };





  /// \f$ \nabla Q (3,ZORDER) \f$
  template <int ZORDER>
  class FE_TNedelecPrism3NoGrad : public HCurlFiniteElement<3>
  {
    ///
// static Array<IPData> ipdata;
// bool ipdatadestructed;
///
static Matrix<> trans_quad;
    ///
static Matrix<> trans_trig;

    FE_NedelecPrism1 prism1;
    FE_NedelecTrig3 trig3;
    FE_Trig2 h1trig2;
    FE_Trig3Pot h1trig3;
    FE_TSegmL2<ZORDER> segm;
  public:
    //  enum { NDOF = 12 * (ZORDER+1) + 10 * ZORDER };
    //  enum { NEDGEDOF = 12 + 3 * (ZORDER-1) };
    // 12 z + 12 + 10 z - 12 - 3z + 3 = 19 z + 3
    enum { NDOF = 19 * ZORDER + 3 };
    enum { NQUADFACEDOF = 3 * (5*ZORDER-3) };
    enum { NTRIGFACEDOF = 6 };
    enum { MAXORDER = (3 > ZORDER) ? 3 : ZORDER };
    // enum { NINNERDOF = 3 * (ZORDER-1) + ZORDER };
    enum { NINNERDOF = 3 * (ZORDER-1) + 1 };

    ///
    FE_TNedelecPrism3NoGrad();
    ///
    virtual ~FE_TNedelecPrism3NoGrad();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcShape1 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcShape2 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    /// quad face dofs
    virtual void CalcShape3 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    /// trig face dofs + inner dofs
    virtual void CalcShape4 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void CalcInner (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void GetInternalDofs (Array<int> & idofs) const;

    ///
    void Orthogonalize();
  };














  ///
  class FE_NedelecPyramid1 : public HCurlFiniteElement<3>
  {
    ///
// static Array<IPData> ipdata;
// bool ipdatadestructed;
///
static Matrix<> trans;
  public:
    ///
    FE_NedelecPyramid1();
    ///
    virtual ~FE_NedelecPyramid1();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<3> shape) const;

    virtual void CalcShape1 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    ///
    void Orthogonalize();

  };



  ///
  class FE_NedelecPyramid2 : public HCurlFiniteElement<3>
  {
  public:
    enum { NDOF = 20 };
    enum { NEDGEDOF = 8 };

  private:
    ///
    // static Array<IPData> ipdata;
    // bool ipdatadestructed;
    ///
    static Matrix<> trans;
    static Matrix<> trans2;
    static Matrix<> trans3;
  
    ///
    // static class FE_Quad1 quad1;
    // static class FE_Quad2 quad2;
    typedef FE_Quad1 quad1;
    typedef FE_Quad2 quad2;

    FE_NedelecPyramid1 pyramid1;
  public:
    ///
    FE_NedelecPyramid2();
    ///
    virtual ~FE_NedelecPyramid2();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<3> shape) const;

    virtual void CalcShape1 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    virtual void CalcShape2 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;


    ///
    void Orthogonalize();

  };


  /// quad of order 3
  class FE_Quad3 : public ScalarFiniteElement<2>
  {
    // static IPDataArray ipdata;

  public:
    FE_Quad3();
    virtual ~FE_Quad3();
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatVector<> shape) const;
    virtual void CalcDShape (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<2> dshape) const;
  }; 



  ///
  class FE_NedelecPyramid3 : public HCurlFiniteElement<3>
  {
  public:
    enum { NDOF = 57 };
    enum { NEDGEDOF = 16 };
    enum { NFACEDOF = 24 };
    enum { NINNERDOF = 9 };
  private:
    ///
    // static Array<IPData> ipdata;
    // bool ipdatadestructed;
    ///
    static Mat<NDOF> trans;
    static Mat<NEDGEDOF> trans2;
    static Mat<NFACEDOF> trans3;
  
    ///
    // static class FE_Quad1 quad1;
    // static class FE_Quad2 quad2;
    typedef FE_Quad1 quad1;
    typedef FE_Quad2 quad2;

    FE_Quad3 quad3;
    FE_NedelecPyramid1 pyramid1;
  public:
    ///
    FE_NedelecPyramid3();
    ///
    virtual ~FE_NedelecPyramid3();
    ///
    virtual void CalcShape (const IntegrationPoint & ip, 
			    FlatMatrixFixWidth<3> shape) const;

    virtual void CalcShape1 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    virtual void CalcShape2 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    virtual void CalcShape3 (const IntegrationPoint & ip, 
			     FlatMatrixFixWidth<3> shape) const;

    ///
    virtual void GetInternalDofs (Array<int> & idofs) const;
    ///
    void Orthogonalize();
  };





  /*
 ///
 class FE_NedelecPyramid3NoGrad : public HCurlFiniteElement<3>
 {
 public:
 //  enum { NDOF = 57 };
 // enum { NEDGEDOF = 16 };
 enum { NDOF = 41 };
 enum { NFACEDOF = 24 };
 enum { NINNERDOF = 9 };
 private:
 ///
 static Array<IPData> ipdata;
 ///
 static Mat<NDOF> trans;
 // static Mat<NEDGEDOF> trans2;
 static Mat<NFACEDOF> trans3;
  
 ///
 FE_Quad1 quad1;
 FE_Quad2 quad2;
 FE_Quad3 quad3;
 FE_NedelecPyramid1 pyramid1;
 public:
 ///
 FE_NedelecPyramid3();
 ///
 virtual ~FE_NedelecPyramid3();
 ///
 virtual void CalcShape (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<3> shape) const;

 virtual void CalcShape1 (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<3> shape) const;

 virtual void CalcShape2 (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<3> shape) const;

 virtual void CalcShape3 (const IntegrationPoint & ip, 
 FlatMatrixFixWidth<3> shape) const;

 ///
 virtual void GetInternalDofs (Array<int> & idofs) const;
 ///
 void Orthogonalize();
 };
  */

}


#endif
