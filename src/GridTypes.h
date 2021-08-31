#ifndef GRID_TYPES_H
#define GRID_TYPES_H

/*!
  \file
  \brief ƒOƒŠƒbƒhŒ^‚Ì’è‹`

  \author Satofumi KAMIMURA

  $Id$
*/


namespace beego {
  /*!
    \brief Šp“xƒNƒ‰ƒX
  */
  class Angle {
    double radian;

    friend Angle deg(const double degree);
    friend Angle rad(const double radian);

  public:
    Angle(void);
    Angle(const Angle& rhs);
    Angle& operator = (const Angle& rhs);
    ~Angle(void);

    double to_rad(void) const;
    double to_deg(void) const;
  };


  /*!
    \brief ‚QŸŒ³ˆÊ’u
  */
  template<class T> class Grid {
  public:
    T x;
    T y;

    Grid(void) : x(0), y(0) {
    }

    Grid(const Grid& rhs) : x(rhs.x), y(rhs.y) {
    }

    Grid& operator = (const Grid& rhs) {
      this->x = rhs.x;
      this->y = rhs.y;

      return *this;
    }

    Grid(const T x, const T y) : x(x), y(y) {
    }

    ~Grid(void) {
    }
  };


  /*!
    \brief ‚QŸŒ³‚ÌˆÊ’uAŒü‚«
  */
  template<class T> class Position {
  public:
    T x;
    T y;
    Angle angle;

    Position(void) : x(0), y(0) {
    }

    Position(const int x, const int y, const Angle angle)
      : x(x), y(y), angle(angle) {
    }

    Position(const Position& rhs) : x(rhs.x), y(rhs.y), angle(rhs.angle) {
    }

    Position& operator = (const Position& rhs) {
      this->x = rhs.x;
      this->y = rhs.y;
      this->angle = rhs.angle;

      return *this;
    }

    ~Position(void) {
    }
  };


  /*!
    \brief ‚RŸŒ³ˆÊ’u
  */
  template<class T> class Grid3D {
  public:
    T x;
    T y;
    T z;

    Grid3D(void) : x(0), y(0), z(0) {
    }

    Grid3D(const Grid3D& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {
    }

    Grid3D& operator = (const Grid3D& rhs) {
      this->x = rhs.x;
      this->y = rhs.y;
      this->z = rhs.z;

      return *this;
    }

    ~Grid3D(void) {
    }
  };

  /*!
    \brief degree Šp“x‚Ì•ÏŠ·

    \param degree [i] degree Šp“x
    \return Angle Šp“x
  */
  extern Angle deg(const double degree);

  /*!
    \brief radian Šp“x‚Ì•ÏŠ·

    \param radian [i] radian Šp“x
    \return Angle Šp“x
  */
  extern Angle rad(const double radian);


  /*!
    \brief ‚QŸŒ³ˆÊ’uAŒü‚«‚Ì‘ã“ü
  */
  template<class T> void set_Position(Position<T>* obj, T x, T y,
                                      const Angle& angle) {
    obj->x = x;
    obj->y = y;
    obj->angle = angle;
  }


  /*!
    \brief ‚QŸŒ³ˆÊ’u‚Ì‘ã“ü
  */
  template<class T> void set_Grid(Grid<T>* obj, T x, T y) {
    obj->x = x;
    obj->y = y;
  }


  /*!
    \brief ‚RŸŒ³ˆÊ’u‚Ì‘ã“ü
  */
  template<class T> void set_Grid3D(Grid3D<T>* obj, T x, T y, T z) {
    obj->x = x;
    obj->y = y;
    obj->z = z;
  }
};

#endif /* !GRID_TYPES_H */
