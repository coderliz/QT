load(qttest_p4)
SOURCES  += tst_qchar.cpp

QT = core

wince*: {
deploy.sources += NormalizationTest.txt
DEPLOYMENT = deploy
}

