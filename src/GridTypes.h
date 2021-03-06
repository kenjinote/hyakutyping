#ifndef GRID_TYPES_H
#define GRID_TYPES_H

/*!
  \file
  \brief グリッド型の定義

  \author Satofumi KAMIMURA

  $Id$
*/


namespace beego {
  /*!
    \brief 角度クラス
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
    \brief ２次元位置
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
    \brief ２次元の位置、向き
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
    \brief ３次元位置
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
    \brief degree 角度の変換

    \param degree [i] degree 角度
    \return Angle 角度
  */
  extern Angle deg(const double degree);

  /*!
    \brief radian 角度の変換

    \param radian [i] radian 角度
    \return Angle 角度
  */
  extern Angle rad(const double radian);


  /*!
    \brief ２次元位置、向きの代入
  */
  template<class T> void set_Position(Position<T>* obj, T x, T y,
                                      const Angle& angle) {
    obj->x = x;
    obj->y = y;
    obj->angle = angle;
  }


  /*!
    \brief ２次元位置の代入
  */
  template<class T> void set_Grid(Grid<T>* obj, T x, T y) {
    obj->x = x;
    obj->y = y;
  }


  /*!
    \brief ３次元位置の代入
  */
  template<class T> void set_Grid3D(Grid3D<T>* obj, T x, T y, T z) {
    obj->x = x;
    obj->y = y;
    obj->z = z;
  }
};

#endif /* !GRID_TYPES_H */
