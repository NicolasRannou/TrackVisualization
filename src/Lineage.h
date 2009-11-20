#ifndef __Lineage_h
#define __Lineage_h

#include <vector>

/**
\class Lineage
\brief 
*/
template< class TTrack >
class Lineage
{
  public:
    typedef TTrack TrackType;
    typedef Lineage Self;


    /** \brief Constructor */
    Lineage( Self* iMother = 0, TrackType* iTrack = 0) : m_Mother( iMother ), 
      m_Daughter1( 0 ), m_Daughter2( 0 ), m_Track( iTrack )
      {}
      
    /** \brief Constructor */
    Lineage( Self* iMother, Self* iD1, Self* iD2, TrackType* iTrack ) : 
      m_Mother( iMother ), m_Daughter1( iD1), m_Daughter2( iD2), m_Track( iTrack )
      {

      }
      
     /** \brief Desctructor */
    ~Lineage() {}
    
    void SetTrack( TrackType* iTrack )
      {
      m_Track = iTrack;
      }
      
    TrackType* GetTrack() const
      {
      return m_Track;
      }
    
    /** \brief    */
    bool IsRoot() const
      {
      return ( m_Mother == 0 );
      }
      
    /** \brief    */
    bool IsLeaf() const
      {
      return ( m_Daughter1 == 0 ) && ( m_Daughter2 == 0 );
      }
      
    /** \brief    */
    Self* GetMother() const
      {
      return m_Mother;
      }
    /** \brief    */
    void SetMother( Self* i_Mother )
      {
      m_Mother = i_Mother;
      }  
    
    /** \brief    */
    Self* GetDaughter1() const
      {
      return m_Daughter1;
      }
    /** \brief    */
    void SetDaughter1( Self* iD1 )
      {
      m_Daughter1 = iD1;
      }
      
    /** \brief    */
    Self* GetDaughter2() const
      {
      return m_Daughter2;
      }
    /** \brief    */
    void SetDaughter2( Self* iD2 )
      {
      m_Daughter2 = iD2;
      }
      
    /** \brief    */
    //std::vector< Self* > GetDaughters() const
    //  {
    //  return m_Daughter;
    //  }
    /** \brief    */
    //void SetDaughters( const std::vector< Self* >& iD )
    //  {
    //  if( iD.size() == 2 )
    //    {
    //    m_Daughter = iD;
    //    }
    //  }
      
  protected:
    Self* m_Mother;
    Self* m_Daughter1;
    Self* m_Daughter2;
    TrackType* m_Track;
    
  private:
    Lineage( const Self& );
    void operator = ( const Self& );
};
#endif
