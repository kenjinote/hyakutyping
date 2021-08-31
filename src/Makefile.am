## Makefile.am -- Process this file with automake to produce Makefile.in

bin_PROGRAMS = hyakutyping
AM_CXXFLAGS = -DPACKAGE_DATA_DIR='"${pkgdatadir}"'
hyakutyping_LDADD = -lSDL_ttf -lSDL_mixer -lSDL_image -lSDL_net

hyakutypingdir = ${datadir}/hyakutyping
hyakutyping_DATA = \
	${top_srcdir}/resource/font.ttf \
	${top_srcdir}/resource/music1.mp3 \
	${top_srcdir}/resource/music2.mp3 \
	${top_srcdir}/resource/effect1.wav \
	${top_srcdir}/resource/effect2.wav \
	${top_srcdir}/resource/decide.wav \
	${top_srcdir}/resource/miss.wav \
	${top_srcdir}/voicedata/cd_tanakarie.txt \
	${top_srcdir}/voicedata/cd_makiidumi.txt \
	${top_srcdir}/voicedata/waka_tanakarie.txt \
	${top_srcdir}/voicedata/waka_makiidumi.txt


EXTRA_DIST = ${top_srcdir}/resource/font.ttf \
	${top_srcdir}/resource/music1.mp3 \
	${top_srcdir}/resource/music2.mp3 \
	${top_srcdir}/resource/effect1.wav \
	${top_srcdir}/resource/effect2.wav \
	${top_srcdir}/resource/decide.wav \
	${top_srcdir}/resource/miss.wav \
	${top_srcdir}/voicedata/cd_wakamotonorio.txt \
	${top_srcdir}/voicedata/cd_tanakarie.txt \
	${top_srcdir}/voicedata/cd_makiidumi.txt \
	${top_srcdir}/voicedata/waka_wakamotonorio.txt \
	${top_srcdir}/voicedata/waka_tanakarie.txt \
	${top_srcdir}/voicedata/waka_makiidumi.txt \
	${top_srcdir}/src/hyakutyping.rc \
	${top_srcdir}/src/hyakutyping.ico \
	${top_srcdir}/src/ReadMe.txt

hyakutyping_SOURCES = hyakutyping.h hyakutyping.cpp CommonResources.h CommonResources.cpp FirstMenu.h FirstMenu.cpp SdlVideo.h SdlVideo.cpp Component.h Component.cpp TypingTextConverter.h TypingTextConverter.cpp SurfaceList.h TextConvertInterface.h Surface.h ResourceDefinition.h SystemDefinition.h DrawsDefinition.h AccessSettings.h AccessSettings.cpp TypingRecorder.h TypingRecorder.cpp GuiManager.h GuiManager.cpp Layer.h Layer.cpp InputHandler.h InputHandler.cpp FillSurface.h FillSurface.cpp SdlSurface.h SdlSurface.cpp TtfResource.h TtfResource.cpp TextProperty.h ConsolePanelComponent.h ConsolePanelComponent.cpp InputReceiveComponent.h InputReceiveComponent.cpp LabelComponent.h LabelComponent.cpp SearchFilePath.h SearchFilePath.cpp back_bmp.h CreateSurfaceFromArray.h CreateSurfaceFromArray.cpp MusicManager.h MusicManager.cpp SoundEffectManager.h SoundEffectManager.cpp ExistFile.h ExistFile.cpp DetectOS.h GuiColors.h GuiColors.cpp TypingMode.h ComponentList.h SurfaceInterface.h ComponentInterface.h FirstMenu_uni.h PracticeMenu.h PracticeMenu.cpp NetworkMenu.h NetworkMenu.cpp OptionMenu.h OptionMenu.cpp UserMenu.h UserMenu.cpp user_bmp.h TextSurface.h TextSurface.cpp ButtonComponent.h ButtonComponent.cpp MenuComponent.h MenuComponent.cpp ComboBoxComponent.h ComboBoxComponent.cpp SdlUtils.h SdlUtils.cpp Delay.h Delay.cpp SdlVideoInit.h SdlVideoInit.cpp LockGuard.h LockGuard.cpp LogManager.h LogManager.cpp EvaluateArea.h EvaluateArea.cpp RomanConvertTable.h KanaConvertTable.h UtfString.h UtfString.cpp GetTicks.h GetTicks.cpp AccessProperty.h AccessProperty.cpp hyakutypingrc_orig.h CreateFileFromArray.h CreateFileFromArray.cpp LayerManager.h LayerManager.cpp InputEvent.h InputEvent.cpp SdlInit.h SdlInit.cpp ModifySurface.h ModifySurface.cpp TtfInit.h TtfInit.cpp PointerRingBuffer.h MixerInit.h MixerInit.cpp ThreadCreator.h ThreadCreator.cpp PracticeMenu_uni.h ShimonokuPractice.h ShimonokuPractice.cpp KimarijiPractice.h KimarijiPractice.cpp NetworkMenu_uni.h UserMenu_uni.h CreateUser.h CreateUser.cpp RemoveUser.h RemoveUser.cpp SwitchSurface.h SwitchSurface.cpp GridTypes.h GridTypes.cpp down_triangle.h ColorSurface.h ColorSurface.cpp SdlAudioInit.h SdlAudioInit.cpp ShimonokuPractice_uni.h ShimonokuInput.h ShimonokuInput.cpp ResultDrawer.h ResultDrawer.cpp GraphDrawer.h GraphDrawer.cpp ShuffleWaka.h ShuffleWaka.cpp BaseEntity.h BaseEntity.cpp StateMachine.h State.h KimarijiPractice_uni.h CreateUser_uni.h ImageSurface.h ImageSurface.cpp TextInputComponent.h TextInputComponent.cpp GetAccountName.h GetAccountName.cpp HttpAccess.h HttpAccess.cpp RemoveUser_uni.h MathUtils.h WakaDrawer.h WakaDrawer.cpp WakaData.h RomanCreator.h RomanCreator.cpp ResultDrawer_uni.h AccessRecordPC.h AccessRecordPC.cpp TextFadeSurface.h TextFadeSurface.cpp AccessRecordInterface.h FadeSurfacesInterface.h GraphDrawer_uni.h GraphComponent.h GraphComponent.cpp NullSurface.h NullSurface.cpp PlainTextConverter.h PlainTextConverter.cpp NetInit.h NetInit.cpp TcpipCtrl.h TcpipCtrl.cpp ConnectionInterface.h bs_bmp.h return_bmp.h TextRubiSurface.h TextRubiSurface.cpp FindFiles.h FindFiles.cpp DrawPanelSurface.h DrawPanelSurface.cpp SocketSet.h SocketSet.cpp RingBuffer.h DrawPrimitive.h DrawPrimitive.cpp RemoveFile.h RemoveFile.cpp CreateDirectory.h CreateDirectory.cpp AnimationSurface.h AnimationSurface.cpp selector_00_bmp.h selector_01_bmp.h selector_02_bmp.h selector_03_bmp.h white_bmp.h MultiSurface.h MultiSurface.cpp RedrawSurface.h RedrawSurface.cpp arrow_bmp.h SwitchLabelComponent.h SwitchLabelComponent.cpp SelectComponent.h SelectComponent.cpp ViewHistory.h ViewHistory.cpp CdromPlayer.h CdromPlayer.cpp SdlCdromInit.h SdlCdromInit.cpp OptionMenuComponent.h OptionMenuComponent.cpp PlaceOption.h OptionMenu_uni.h CheckBoxComponent.h CheckBoxComponent.cpp VolumeSliderComponent.h VolumeSliderComponent.cpp check.h PracticeNum_uni.h speaker_02_bmp.h warn_bmp.h hyakutyping_bmp.h icon_mask.h AccessRecordWeb.h AccessRecordWeb.cpp VoicePlayer.h VoicePlayer.cpp UpdateUtils.h UpdateUtils.cpp