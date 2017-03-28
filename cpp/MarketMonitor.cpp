#include <hpp/MarketMonitor.h>


MarketMonitor::MarketMonitor()
{
}


MarketMonitor::~MarketMonitor()
{
}

void MarketMonitor::initialize()
{
	PoloniexClient client;

	//qDebug() << QSslSocket::sslLibraryBuildVersionString();

	//Parse json test.
	std::string pair = "BTC_XMR";
	int start = std::time(0) - 50000;
	int end = std::time(0);
	int seconds = 900;
	//300, 900, 1800, 7200, 14400, and 86400
	//5min  15  30  2hr 4hr 1day




	QCandlestickSeries *acmeSeries = new QCandlestickSeries();
	acmeSeries->setName("Acme Ltd");
	acmeSeries->setIncreasingColor(QColor(125, 249, 255));
	acmeSeries->setDecreasingColor(QColor(Qt::darkMagenta));



	QStringList categories;



	std::string result = client.public_ChartData(pair, seconds, start, end);

	if (result != "FAIL") {
		QString qresult = QString::fromStdString(result);
		QJsonDocument doc = QJsonDocument::fromJson(qresult.toUtf8());
		QJsonArray candles = doc.array();

		for (int i = 0; i<candles.size(); i++) {
			QJsonObject dateobj = candles[i].toObject();

			const qreal timestamp = dateobj["date"].toDouble();
			const qreal open = dateobj["open"].toDouble();
			const qreal high = dateobj["high"].toDouble();
			const qreal low = dateobj["low"].toDouble();
			const qreal close = dateobj["close"].toDouble();

			QCandlestickSet *candlestickSet = new QCandlestickSet(timestamp);
			candlestickSet->setOpen(open);
			candlestickSet->setHigh(high);
			candlestickSet->setLow(low);
			candlestickSet->setClose(close);


			acmeSeries->append(candlestickSet);
			categories << QDateTime::fromMSecsSinceEpoch(candlestickSet->timestamp()).toString("hh:mm:ss");
		}

	}


	QChart *chart = new QChart();
	chart->setTheme(QChart::ChartThemeDark);
	chart->addSeries(acmeSeries);

	chart->setTitle(QString::fromStdString(pair));
	chart->setAnimationOptions(QChart::SeriesAnimations);

	chart->createDefaultAxes();

	QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
	axisX->setCategories(categories);

	QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
	axisY->setMax(axisY->max() * 1.01);
	axisY->setMin(axisY->min() * 0.99);

	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);

	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	chartView->setRubberBand(QChartView::HorizontalRubberBand);


	

	// Connect button signal to appropriate slot
	//connect(button, SIGNAL(released()), this, SLOT(button_update()));


	QWidget *window = new QWidget;
	QVBoxLayout *box = new QVBoxLayout;


	QPushButton * button = new QPushButton();
	button->resize(100, 50);
	button->setText("UpdateChart");
	QObject::connect(button, SIGNAL(clicked()), this, SLOT(button_update()));


	box->addWidget(button);
	box->addWidget(chartView);
	window->resize(800, 600);
	window->setLayout(box);
	window->show();

	qDebug() << "MarketMonitor Created";

}


void MarketMonitor::button_update() {

	qDebug() << "button clicked";

}