#pragma once

#include <QRunnable>
#include "TalkWindowShell.h"

//用于替代本来TalkWindowShell里面的 onProcessPendingData 
class PoolTask  :public TalkWindowShell, public QRunnable
{
	Q_OBJECT

public:
	PoolTask(TalkWindowShell *parent= nullptr);
	~PoolTask();

public:
	void run() override;
};
