#include <stdint.h>
#include "../common/comm.h"
#include "capture_op.h"
CaptureOp::CaptureOp(RenderPosition type,int32_t index):_type(type),_index(index)
{
    _forward=true;
}
RenderPosition CaptureOp::GetType()
{
    return _type;
}
bool CaptureOp::IsForward()
{
    return _forward;
}
void CaptureOp::SetForward(bool b)
{
    _forward=b;
}
int32_t CaptureOp::GetIndex()
{
    return _index;
}
 