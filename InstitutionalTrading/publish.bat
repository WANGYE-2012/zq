
rem modify file version to 1.0.0.mmddhhmm

rem copy client

rem copy Risk clent
copy Release\cfg.xml,publish\����\����ն�\cfg.xml
copy Release\CltRisk.exe,publish\����\����ն�\CltRisk.exe
copy Release\IPControl.dll,publish\����\����ն�\IPControl.dll
copy Release\CurrencyBox.dll,publish\����\����ն�\CurrencyBox.dll
copy Release\AnywndComboBox.dll,publish\����\����ն�\AnywndComboBox.dll
copy Release\SimpleReportControl.dll,publish\����\����ն�\SimpleReportControl.dll
copy Release\TriTreeControl.dll,publish\����\����ն�\TriTreeControl.dll
copy Release\WeifenLuo.WinFormsUI.Docking.dll,publish\����\����ն�\WeifenLuo.WinFormsUI.Docking.dll

rem copy SeverMgr clent
copy Release\cfg.xml,publish\����\��̨�����ն�\cfg.xml
copy Release\CltServerMgr.exe,publish\����\��̨�����ն�\CltServerMgr.exe
copy Release\IPControl.dll,publish\����\��̨�����ն�\IPControl.dll
copy Release\CurrencyBox.dll,publish\����\��̨�����ն�\CurrencyBox.dll
copy Release\AnywndComboBox.dll,publish\����\��̨�����ն�\AnywndComboBox.dll
copy Release\SimpleReportControl.dll,publish\����\��̨�����ն�\SimpleReportControl.dll
copy Release\TriTreeControl.dll,publish\����\��̨�����ն�\TriTreeControl.dll
copy Release\WeifenLuo.WinFormsUI.Docking.dll,publish\����\��̨�����ն�\WeifenLuo.WinFormsUI.Docking.dll

rem copy Presentation clent
copy Release\cfg.xml,publish\����\չ���ն�\cfg.xml
copy Release\CltPresentation.exe,publish\����\չ���ն�\CltPresentation.exe
copy Release\AnywndComboBox.dll,publish\����\չ���ն�\AnywndComboBox.dll
copy Release\SimpleReportControl.dll,publish\����\չ���ն�\SimpleReportControl.dll
copy Release\CFMap.Core.dll,publish\����\չ���ն�\CFMap.Core.dll
copy Release\CFMap.WinForm.Control.dll,publish\����\չ���ն�\CFMap.WinForm.Control.dll
copy Release\ListPanel.dll,publish\����\չ���ն�\ListPanel.dll
copy Release\Microsoft.Office.Interop.Excel.dll,publish\����\չ���ն�\Microsoft.Office.Interop.Excel.dll
copy Release\Microsoft.Vbe.Interop.dll,publish\����\չ���ն�\Microsoft.Vbe.Interop.dll
copy Release\Office.dll,publish\����\չ���ն�\Office.dll
copy Release\SimpleGradientProgressBar.dll,publish\����\չ���ն�\SimpleGradientProgressBar.dll
copy Release\SimpleGuage.dll,publish\����\չ���ն�\SimpleGuage.dll
copy Release\SimpleFlagControl.dll,publish\����\չ���ն�\SimpleFlagControl.dll
copy Release\System.Data.SQLite.DLL,publish\����\չ���ն�\System.Data.SQLite.DLL

rem copy strateay manage client
copy Server\RunSingleStrategy\bin\Release\RunSingleStrategy.exe,publish\����\���Թ����ն�\SupportFiles\RunSingleStrategy.exe

rem copy server
cd..
copy Miscellaneous\uniServer\binRelease\SvrTSS.dll,InstitutionalTrading\publish\����\���׷�����\SvrTSS.dll
copy Miscellaneous\uniServer\binRelease\SvrRisk.dll,InstitutionalTrading\publish\����\���׷�����\SvrRisk.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeExcute.dll,InstitutionalTrading\publish\����\���׷�����\SvrTradeExcute.dll
copy Miscellaneous\uniServer\binRelease\SvrStatistics.dll,InstitutionalTrading\publish\����\���׷�����\SvrStatistics.dll
copy Miscellaneous\uniServer\binRelease\SvrSettlement.dll,InstitutionalTrading\publish\����\���׷�����\SvrSettlement.dll
copy Miscellaneous\uniServer\binRelease\SvrOffer.dll,InstitutionalTrading\publish\����\���׷�����\SvrOffer.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeData.dll,InstitutionalTrading\publish\����\���׷�����\SvrTradeData.dll
copy Miscellaneous\uniServer\binRelease\SvrTradingStrategy.dll,InstitutionalTrading\publish\����\���׷�����\SvrTradingStrategy.dll
copy Miscellaneous\uniServer\binRelease\SvrMsg.dll,InstitutionalTrading\publish\����\���׷�����\SvrMsg.dll
copy Miscellaneous\uniServer\binRelease\SvrFinanceProduct.dll,InstitutionalTrading\publish\����\���׷�����\SvrFinanceProduct.dll
copy Miscellaneous\uniServer\binRelease\SvrLogin.dll,InstitutionalTrading\publish\����\���׷�����\svrLogin.dll
copy Miscellaneous\uniServer\binRelease\SvrUserOrg.dll,InstitutionalTrading\publish\����\���׷�����\SvrUserOrg.dll
copy Miscellaneous\uniServer\binRelease\SvrNotifyAndAsk.dll,InstitutionalTrading\publish\����\���׷�����\SvrNotifyAndAsk.dll
copy Miscellaneous\uniServer\binRelease\SvrBrokerInfo.dll,InstitutionalTrading\publish\����\���׷�����\SvrBrokerInfo.dll 
copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\����\���׷�����\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrTemplate.dll,InstitutionalTrading\publish\����\���׷�����\SvrTemplate.dll
copy Miscellaneous\uniServer\binRelease\SvrPresentation.dll,InstitutionalTrading\publish\����\���׷�����\SvrPresentation.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\����\���׷�����\SvrDBOpr.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\����\���׷�����\UniServer.exe

copy Miscellaneous\uniServer\binRelease\SvrTSS.dll,InstitutionalTrading\publish\����\���Է�����\SvrTSS.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeExcute.dll,InstitutionalTrading\publish\����\���Է�����\SvrTradeExcute.dll
copy Miscellaneous\uniServer\binRelease\SvrOffer.dll,InstitutionalTrading\publish\����\���Է�����\SvrOffer.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeData.dll,InstitutionalTrading\publish\����\���Է�����\SvrTradeData.dll
copy Miscellaneous\uniServer\binRelease\SvrTradingStrategy.dll,InstitutionalTrading\publish\����\���Է�����\SvrTradingStrategy.dll
copy Miscellaneous\uniServer\binRelease\SvrMsg.dll,InstitutionalTrading\publish\����\���Է�����\SvrMsg.dll
copy Miscellaneous\uniServer\binRelease\SvrFinanceProduct.dll,InstitutionalTrading\publish\����\���Է�����\SvrFinanceProduct.dll
copy Miscellaneous\uniServer\binRelease\SvrLogin.dll,InstitutionalTrading\publish\����\���Է�����\svrLogin.dll
copy Miscellaneous\uniServer\binRelease\SvrStrategy.dll,InstitutionalTrading\publish\����\���Է�����\SvrStrategy.dll
copy Miscellaneous\uniServer\binRelease\SvrUserOrg.dll,InstitutionalTrading\publish\����\���Է�����\SvrUserOrg.dll
copy Miscellaneous\uniServer\binRelease\SvrNotifyAndAsk.dll,InstitutionalTrading\publish\����\���Է�����\SvrNotifyAndAsk.dll
copy Miscellaneous\uniServer\binRelease\SvrBrokerInfo.dll,InstitutionalTrading\publish\����\���Է�����\SvrBrokerInfo.dll 
copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\����\���Է�����\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrTemplate.dll,InstitutionalTrading\publish\����\���Է�����\SvrTemplate.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\����\���Է�����\SvrDBOpr.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\����\���Է�����\UniServer.exe
copy Miscellaneous\uniServer\binRelease\SvrRedis.dll,InstitutionalTrading\publish\����\���Է�����\SvrRedis.dll


copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\����\��ط�����\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\����\��ط�����\SvrDBOpr.dll
copy Miscellaneous\uniServer\binRelease\SvrNotifyAndAsk.dll,InstitutionalTrading\publish\����\��ط�����\SvrNotifyAndAsk.dll
copy Miscellaneous\uniServer\binRelease\SvrBrokerInfo.dll,InstitutionalTrading\publish\����\��ط�����\SvrBrokerInfo.dll 
copy Miscellaneous\uniServer\binRelease\SvrUserOrg.dll,InstitutionalTrading\publish\����\��ط�����\SvrUserOrg.dll
copy Miscellaneous\uniServer\binRelease\SvrFinanceProduct.dll,InstitutionalTrading\publish\����\��ط�����\SvrFinanceProduct.dll
copy Miscellaneous\uniServer\binRelease\SvrMsg.dll,InstitutionalTrading\publish\����\��ط�����\SvrMsg.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeData.dll,InstitutionalTrading\publish\����\��ط�����\SvrTradeData.dll
copy Miscellaneous\uniServer\binRelease\SvrRisk.dll,InstitutionalTrading\publish\����\��ط�����\SvrRisk.dll
copy Miscellaneous\uniServer\binRelease\SvrLogin.dll,InstitutionalTrading\publish\����\��ط�����\svrLogin.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\����\��ط�����\UniServer.exe



rem goto qutoserver
rem :qutoserver
copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\����\���������\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\����\���������\SvrDBOpr.dll
copy Miscellaneous\uniServer\binRelease\SvrQuotServer.dll,InstitutionalTrading\publish\����\���������\SvrQuotServer.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\����\���������\UniServer.exe
