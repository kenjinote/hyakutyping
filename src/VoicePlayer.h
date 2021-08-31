#ifndef VOICE_PLAYER_H
#define VOICE_PLAYER_H

/*!
  \file
  \brief òaâÃÇÃârÇ›è„Ç∞

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>
#include <string>
#include <vector>


class VoicePlayer {
  VoicePlayer(void);
  VoicePlayer(const VoicePlayer& rhs);
  VoicePlayer& operator = (const VoicePlayer& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  VoicePlayer(const char* directory);
  ~VoicePlayer(void);

  std::vector<std::string> getResources(void);
  void setMute(void);
  bool checkResource(std::string resource = "");
  void play(size_t id);
  void stop(void);
};

#endif /* !VOICE_PLAYER_H */
