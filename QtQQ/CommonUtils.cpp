#include "CommonUtils.h"
#include <QPainter>
#include <QFile>
#include <QWidget>
#include <QApplication>
#include <QSettings>

CommonUtils::CommonUtils()
{
}

//获取圆头像
QPixmap CommonUtils::getRoundImage(const QPixmap &src, QPixmap &mask, QSize masksize)
{
	if (masksize == QSize(0, 0))
	{
		masksize = mask.size();
	}
	else
	{
		mask = mask.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation);//scaled 缩放
	}

	QImage resultImage(masksize, QImage::Format_ARGB32_Premultiplied);//resultImage保存处理结果
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);//设置兼容性
	painter.fillRect(resultImage.rect(), Qt::transparent);//填充矩形
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);//图片进行重叠
	painter.drawPixmap(0, 0, mask);//画图
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();
	return QPixmap::fromImage(resultImage);
}

//加载样式表
void CommonUtils::loadStyleSheet(QWidget* widget, const QString &sheetName)
{
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		widget->setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());
		widget->setStyleSheet(qsstyleSheet);
	}
	file.close();
}

//设置皮肤颜色
void CommonUtils::setDefaultSkinColor(const QColor &color)
{
	//将配置文件默认的皮肤进行重写
	const QString && path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");//获取文件路径
	QSettings settings(path, QSettings::IniFormat);//读写的方式
	settings.setValue("DefaultSkin/red", color.red());
	settings.setValue("DefaultSkin/green", color.green());
	settings.setValue("DefaultSkin/blue", color.blue());
}

//获取皮肤颜色
QColor CommonUtils::getDefaultSkinColor()
{
	QColor color;
	const QString && path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	if (!QFile::exists(path))
		setDefaultSkinColor(QColor(22, 154, 218));

	QSettings settings(path, QSettings::IniFormat);
	color.setRed(settings.value("DefaultSkin/red").toInt());
	color.setGreen(settings.value("DefaultSkin/green").toInt());
	color.setBlue(settings.value("DefaultSkin/blue").toInt());
	return color;
}