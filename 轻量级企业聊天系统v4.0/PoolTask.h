#pragma once

#include <QRunnable>
#include "TalkWindowShell.h"

//�����������TalkWindowShell����� onProcessPendingData 
class PoolTask  :public TalkWindowShell, public QRunnable
{
	Q_OBJECT

public:
	PoolTask(TalkWindowShell *parent= nullptr);
	~PoolTask();

public:
	void run() override;
};
