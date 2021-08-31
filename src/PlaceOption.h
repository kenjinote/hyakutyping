#ifndef PLACE_OPTION_H
#define PLACE_OPTION_H

/*!
  \file
  \brief 配置オプションの定義

  \author Satofumi KAMIMURA

  \todo いずれ、typedef enum にする、か？
*/

namespace beego {
  enum {
      None = 0x00,              //!< 指定なし
      Top = 0x01,               //!< 上
      Middle = 0x02,            //!< 中
      Bottom = 0x04,            //!< 下
      Left = 0x08,              //!< 左
      Center = 0x10,            //!< 中央
      Right = 0x20,             //!< 右
  };
};

#endif /* !PLACE_OPTION_H */
