#ifndef __EASYPROJECTOR_H__
#define __EASYPROJECTOR_H__

#include <libtderandr/libtderandr.h>

#include <tqhbox.h>

class TDEProcess;
class BigLabel;
class BigButton;

typedef TQPtrList<SingleScreenData> ScreenArray;

class EasyProjector : public TQHBox
{
  TQ_OBJECT

  enum ScreenMode { PC_Only, Duplicate, Extend };

  public:
    EasyProjector(bool horizontal = false);
    ~EasyProjector();

  private:
    KRandrSimpleAPI  *m_simplerandr;
    ScreenArray       m_screens;
    SingleScreenData *m_primary   = 0;
    SingleScreenData *m_projector = 0;
    int               m_side;

    BigLabel         *m_state;
    BigButton        *m_pc_only;
    BigButton        *m_duplicate;
    BigButton        *m_extend;

    void setButtonsState(bool on);
    TQString bestProjectorResolution();

  private slots:
    ScreenMode m_mode;
    void updateState();
    void togglePCOnly(bool on);
    void toggleDuplicate(bool on);
    void toggleExtend(bool on);
    void receivedStderr(TDEProcess *proc, char *buffer, int bufflen);

};

#endif // __EASYPROJECTOR_H__
