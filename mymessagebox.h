#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QWidget>
#include "ui_mymessagebox.h"
#include "basewindow.h"

enum ChosseResult
{
	ID_OK = 0,						// ȷ��;
	ID_CANCEL						// ȡ��;
};

enum MessageType
{
	MESSAGE_INFORMATION = 0,		// ��ʾ��Ϣ;
	MESSAGE_WARNNING,				// ��ʾ����;
	MESSAGE_QUESTION,				// ��ʾѯ��;
	MESSAGE_INPUT					// ��ʾ�����;
};

enum MessageButtonType
{
	BUTTON_OK = 0,					// ֻ��ȷ����ť;
	BUTTON_OK_AND_CANCEL,			// ȷ����ȡ����ť;
	BUTTON_CLOSE					// �رհ�ť;
};

class MyMessageBox : public BaseWindow
{
	Q_OBJECT

public:
	MyMessageBox(QWidget *parent = 0);
	~MyMessageBox();

	void setWindowTitle(QString title, int titleFontSize = 10);
	void setContentText(QString contentText);
	void setMessageType(MessageType messageType);
	void setButtonType(MessageButtonType buttonType);
	void setMessageContent(QString messageContent);

public:
	int static showMyMessageBox(QWidget* parent, const QString &title,const QString &contentText , MessageType messageType, MessageButtonType messageButtonType , bool isModelWindow = false);


private:
	void initWindow();
	void initTitleBar();
	int exec();

	void paintEvent(QPaintEvent *event);
	void closeEvent(QCloseEvent *event);

private slots:
	void onOkClicked();
	void onCancelClicked();

private:
	Ui::MyMessageBox ui;
	QEventLoop* m_eventLoop;
	ChosseResult m_chooseResult;
};

#endif // MYMESSAGEBOX_H
