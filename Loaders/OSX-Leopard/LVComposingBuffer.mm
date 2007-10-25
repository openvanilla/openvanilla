#import "LVComposingBuffer.h"
#import "LVInputMethodController.h"

LVComposingBuffer::LVComposingBuffer(LVInputMethodController *controller) : TSComposingBuffer(NULL), m_controller(controller)
{
}

TSComposingBuffer* LVComposingBuffer::clear()
{
	[m_controller composingBufferClear];
	return this;
}

TSComposingBuffer* LVComposingBuffer::send()
{
	[m_controller composingBufferSend];
	return this;
}

TSComposingBuffer* LVComposingBuffer::update(Boolean send, int cursor, int markFrom, int markTo)
{
	[m_controller composingBufferUpdate:send cursorPosition:cursor highlightFrom:markFrom to:markTo];
	return this;
}

TSComposingBuffer* LVComposingBuffer::append(NSString* s)
{
	[m_controller composingBufferAppend:s];
	return this;
}

Boolean LVComposingBuffer::isEmpty()
{
	return [m_controller isComposingBufferEmpty];
}

Point LVComposingBuffer::getAppCursorPosition()
{
	return [m_controller getCursorPosition];
}

NSMutableString* LVComposingBuffer::getContent()
{
	return [m_controller composingBufferContent];
}
