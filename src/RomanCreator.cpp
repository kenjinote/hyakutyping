/*!
  \file
  \brief ローマ字入力候補への変換

  \author Satofumi KAMIMURA

  $Id$
*/

#include "RomanCreator.h"
#include "RomanConvertTable.h"
#include "UtfString.h"
#include <string>

using namespace beego;


struct RomanCreator::pImpl {
  const Uint16* convert_table;
  unsigned int max_length;
  std::basic_string<Uint16> converted;
  std::vector<Uint16> converted_char;

  pImpl(const Uint16* convertTable, unsigned int maxLength)
    : convert_table(convertTable), max_length(maxLength) {
  }

  size_t ptnlen(const int index, const Uint16* table) {
    int length = 0;
    while (table[index + length] != 0x0) {
      ++length;
    }
    return length;
  }

  size_t replace(size_t match_index, int table_index) {
    size_t ptn_length = ptnlen(table_index, convert_table);
    converted.replace(match_index, ptn_length,
                      &convert_table[table_index - max_length]);

    return ptn_length - ptnlen(table_index - max_length, convert_table);
  }
};


RomanCreator::RomanCreator(void)
  : pimpl(new pImpl(RomanTable[0][0], ROMAN_CONVERT_SIZE_MAX)) {
}


RomanCreator::~RomanCreator(void) {
}


void RomanCreator::convert(std::vector<Uint16>& dst, const Uint16* input) {
  pimpl->converted.clear();

  // 変換文字列を代入
  size_t input_length = ustrlen(input);
  for (size_t i = 0; i < input_length; ++i) {
    pimpl->converted.push_back(input[i]);
  }

  int steps = pimpl->max_length * 2;
  for (long i = static_cast<int>(pimpl->converted.size()) -1; i >= 0; --i) {
    // 一致したパターンを置換する
    for (int j = pimpl->max_length;
         pimpl->convert_table[j] != 0x0; j += steps) {
      int ptn_length = static_cast<int>(pimpl->ptnlen(j, pimpl->convert_table));
      int compare_first = i - (ptn_length -1);
      if (compare_first < 0) {
        continue;
      }
      size_t match =
        pimpl->converted.compare(compare_first, ptn_length,
                                 &pimpl->convert_table[j]);
      if (match == 0) {
        pimpl->replace(compare_first, j);

        // nba -> んば、といった変換用のために、１文字分戻る
        ++i;
        break;
      }
    }
  }
  pimpl->converted.push_back(0x0);

  pimpl->converted_char.clear();
  for (size_t i = 0; i < pimpl->converted.size(); ++i) {
    pimpl->converted_char.
      push_back(static_cast<unsigned char>(pimpl->converted[i]));
  }
  dst = pimpl->converted_char;
}
