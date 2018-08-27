#include "mymessagebox.h"
#include <QPainter>
#include <QCloseEvent>

MyMessageBox::MyMessageBox(QWidget *parent)
	: BaseWindow(parent)
	, m_eventLoop(NULL)
	, m_chooseResult(ID_CANCEL)
{
	ui.setupUi(this);
	initWindow();
}

MyMessageBox::~MyMessageBox()
{

}

void MyMessageBox::initWindow()
{
	initTitleBar();
    loadStyleSheet("MyMessageBox/MyMessageBox");
	Qt::WindowFlags flags = this->windowFlags();
	this->setWindowFlags(flags | Qt::Window);

	ui.inputContent->setVisible(false);

	connect(ui.pButtonOk, SIGNAL(clicked()), this, SLOT(onOkClicked()));
//	connect(ui.pButtonCancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
}

// ��ʼ��������;
void MyMessageBox::initTitleBar()
{
	m_titleBar->move(0, 0);
    m_titleBar->setWindowBorderWidth(0);
    // ���ñ�������ɫ
    m_titleBar->setBackgroundColor(85, 170, 255);
    m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
    m_titleBar->setTitleWidth(this->width());//this->width()

    // ���ñ�����ͼ��
    m_titleBar->setTitleIcon(":\\Resources\\MyMessageBox\\reminder.png", QSize(24, 24));
}

void MyMessageBox::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	// ���ƴ��ڰ�ɫ����ɫ;
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);
	pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()) , 3 , 3);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(235, 235, 235)));

	// ���ư�ť���ֻ�ɫ����;
	QPainterPath pathButtonBack;
	pathButtonBack.setFillRule(Qt::WindingFill);
	pathButtonBack.addRoundedRect(QRect(0, 116, this->width(), 48) , 3 , 3);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathButtonBack, QBrush(QColor(85, 170, 255)));

	return QWidget::paintEvent(event);
}

void MyMessageBox::setWindowTitle(QString title , int titleFontSize)
{
	m_titleBar->setTitleContent(title, titleFontSize);
}

void MyMessageBox::setContentText(QString contentText)
{
	ui.MessageContent->setText(contentText);
}

void MyMessageBox::setMessageType(MessageType messageType)
{
	switch (messageType)
	{
	case MESSAGE_INFORMATION:
		ui.MessageIcon->setPixmap(QPixmap(":/Resources/MyMessageBox/information.png"));
		break;
	case MESSAGE_WARNNING:
        ui.MessageIcon->setPixmap(QPixmap(":\\Resources\\MyMessageBox\\warnning.png"));
		break;
	case MESSAGE_QUESTION:
		ui.MessageIcon->setPixmap(QPixmap(":/Resources/MyMessageBox/question.png"));
		break;
	case MESSAGE_INPUT:
		ui.MessageIcon->setVisible(false);
		ui.inputContent->setVisible(true);
	default:
		break;
	}
}

void MyMessageBox::setButtonType(MessageButtonType buttonType)
{
	switch (buttonType)
	{
	case BUTTON_OK:
	{
		ui.pButtonOk->setText(QStringLiteral("ȷ��"));
     //	ui.pButtonCancel->setVisible(false);
	}
		break;
	case BUTTON_OK_AND_CANCEL:
	{
		ui.pButtonOk->setText(QStringLiteral("ȷ��"));
        //ui.pButtonCancel->setText(QStringLiteral("ȡ��"));
	}
		break;
	default:
		break;
	}
}

void MyMessageBox::setMessageContent(QString messageContent)
{
	ui.MessageContent->setText(messageContent);
}

int MyMessageBox::showMyMessageBox(QWidget* parent, const QString &title, const QString &contentText, MessageType messageType, MessageButtonType messageButtonType, bool isModelWindow)
{
	MyMessageBox * myMessageBox = new MyMessageBox(parent);
	myMessageBox->setWindowTitle(title);
	myMessageBox->setContentText(contentText);
	myMessageBox->setMessageType(messageType);
	myMessageBox->setButtonType(messageButtonType);
	if (isModelWindow)
	{
		// ����Ϊģ̬����ʱ������parent�������ø�����ָ�룬����ģ̬������Ч;
		// ��Ϊ Qt::WindowModal ����ֻ�Ը�������Ч�������Ҫģ̬��ȫ�ִ��ڶ���Ч�������� Qt::ApplicationModal
		return myMessageBox->exec();
	}
	else
	{
		myMessageBox->show();
	}

	return -1;
}

int MyMessageBox::exec()
{
	// ��ΪQWidgetû��exec()������������Ҫ�Լ����������exec()����;
	// ��exec()��������ֱ�����ô�����ʾΪģ̬�����Ҵ��ڹرս����󷵻��û�ѡ����(����ȷ������ȡ����ť);
	// ��show()����ֻ����ʾ���ڣ����������ô��ڵ�ģ̬���߷�ģ̬����Ҫ�Լ�����setWindowModality()������������;
	// ����show()���������᷵���û�ѡ����;
	// ����Ҳ���Լ̳�QDialog�࣬QDialog���Լ���exec()���������ݷ��� Accepted, Rejected�������Ƿ���ȷ����ť;

	// ����Ϊ���ڼ�ģ̬��Ҳ������ΪӦ�ó���ģ̬ Qt::ApplicationModal;
	this->setWindowModality(Qt::WindowModal); 
	show();
	// ʹ���¼�ѭ��QEventLoop ,����exec()�������������û�ѡ��ȷ������ȡ���󣬹رմ��ڽ����¼�ѭ��������������û�ѡ��Ľ��;
	// ���ݷ��ؽ���õ��û�������ȷ������ȡ������ȡ��Ӧ�Ĳ������Ӷ�ģ���QDialog���exec()����;
	m_eventLoop = new QEventLoop(this);
	m_eventLoop->exec();

	return m_chooseResult;
}

void MyMessageBox::onOkClicked()
{
	m_chooseResult = ID_OK;
	close();
}

void MyMessageBox::onCancelClicked()
{
	m_chooseResult = ID_CANCEL;
	close();
}

void MyMessageBox::closeEvent(QCloseEvent *event)
{
	// �رմ���ʱ�����¼�ѭ������exec()�����з���ѡ����;
	if (m_eventLoop != NULL)
	{
		m_eventLoop->exit();
	}
	event->accept();
}
