/*============================================================================*/
/**  @file       lingual.cpp
 **  @ingroup    user_interface
 **  @brief		 Text defines.
 **
 **  All text in the program.
 **
 **  @author     mensfort
 **
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 7 mrt 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "sdl_graphics.h"
#include "lingual.h"

Taal EuroLang, PrintLang;

/*--Bart Hounkes--17/01/1998---------------------Status:Update continuously--*/
const std::string Lin[MAX_LINTEXT][MAX_LANGUAGE]
	= // number, language { CH, DUTCH, ENG, GER }

	{
	// _JANUAR

	{	"JANUAR", "一月", "JANUARI", "JANUAR", "JANUAR", "JANUARI", "一月" },

	// _FEBRUAR

	{	"FEBRUAR", "二月", "FEBRUARI", "FEBRUAR", "FEBRUARI", "FEBRUARI", "二月" },

	// _MARCH

	{	"MARCH", "三月", "MAART", "MARCH", "MARZ", "MARET", "三月" },

	// _APRIL

	{	"APRIL", "四月", "APRIL", "APRIL", "APRIL", "APRIL", "四月" },

	// _MAY

	{	"MAY", "五月", "MEI", "MAY", "MEI", "MEI", "五月" },

	// _JUNI

	{	"JUNI", "六月", "JUNI", "JUNE", "JUNI", "JUNI", "六月" },

	// _JULY

	{	"JULY", "七月", "JULI", "JULY", "JULI", "JULI", "七月" },

	// _AUGUST

	{	"AUGUST", "八月", "AUGUSTUS", "AUGUST", "AUGUST", "AGUSTUS", "八月" },

	// _SEPTEMBER

	{	"SEPTEMBER", "九月", "SEPTEMBER", "SEPTEMBER", "SEPTEMBER", "SEPTEMBER", "九月" },

	// _OKTOBER

	{	"OKTOBER", "十月", "OKTOBER", "OCTOBER", "OKTOBER", "OKTOBER", "十月"},

	// _NOVEMBER

	{	"NOVEMBER", "十一月", "NOVEMBER", "NOVEMBER", "NOVEMBER", "NOVEMBER", "十一月" },

	// _DECEMBER

	{	"DECEMBER", "十二月", "DECEMBER", "DECEMBER", "DEZEMBER", "DESEMBER", "十二月" },

	// _JANUAR_SHORT

	{	"JANUAR_SHORT", "一月", "JAN", "JAN", "JAN", "JAN", "一月" },

	// _FEBRUAR_SHORT

	{	"FEBRUAR_SHORT", "二月", "FEB", "FEB", "FEB", "FEB", "二月" },

	// _MARCH_SHORT

	{	"MARCH_SHORT", "三月", "MRT", "MAR", "MAR", "MAR", "三月" },

	// _APRIL_SHORT

	{	"APRIL_SHORT", "四月", "APR", "APR", "APR", "APR", "四月" },

	// _MAY_SHORT

	{	"MAY_SHORT", "五月", "MEI", "MAY", "MEI", "MEI", "五月" },

	// _JUNI_SHORT

	{	"JUNI_SHORT", "六月", "JUN", "JUN", "JUN", "JUN", "六月" },

	// _JULY_SHORT

	{	"JULY_SHORT", "七月", "JUL", "JUL", "JUL", "JUL", "七月" },

	// _AUGUST_SHORT

	{	"AUGUST_SHORT", "八月", "AUG", "AUG", "AUG", "AGU", "八月" },

	// _SEPTEMBER_SHORT

	{	"SEPTEMBER_SHORT", "九月", "SEP", "SEP", "SEP", "SEP", "九月" },

	// _OKTOBER_SHORT

	{	"OKTOBER_SHORT", "十月", "OKT", "OCT", "OKT", "OKT", "十月"},

	// _NOVEMBER_SHORT

	{	"NOVEMBER_SHORT", "十一", "NOV", "NOV", "NOV", "NOB", "十一" },

	// _DECEMBER_SHORT

	{	"DECEMBER_SHORT", "十二", "DEC", "DEC", "DEZ", "DES", "十二" },

	// _MONDAY
	{ "MONDAY", "星期一", "MAANDAG", "MONDAY", "MONTAG", "SENIN", "星期一" },

	// _TUESDAY
	{ "TUESDAY", "星期二", "DINSDAG", "TUESDAY", "DIENSTAG", "SELASA", "星期二" },

	// _WEDNESDAY
	{ "WEDNESDAY", "星期三", "WOENSDAG", "WEDNESDAY", "MITTWOCH", "RABU", "星期三" },

	// _THURSDAY
	{ "THURSDAY", "星期四", "DONDERDAG", "THURSDAY", "DONNERSTAG", "KAMIS", "星期四" },

	// _FRIDAY
	{ "FRIDAY", "星期五", "VRIJDAG", "FRIDAY", "FREITAG", "JUMAT", "星期五" },

	// _SATURDAY
	{ "SATURDAY", "星期六", "ZATERDAG", "SATURDAY", "SAMMSTAG", "SABTU", "星期六" },

	// _SUNDAY
	{ "SUNDAY", "星期天", "ZONDAG", "SUNDAY", "SUNTAG", "MINGGU", "星期天" },

	// _MONDAY_SHORT
	{ "MONDAY_SHORT", "一", "MA", "M", "M", "S", "一" },

	// _TUESDAY_SHORT
	{ "TUESDAY_SHORT", "二", "DI", "T", "D", "S", "二" },

	// _WEDNESDAY_SHORT
	{ "WEDNESDAY_SHORT", "三", "WO", "W", "M", "R", "三" },

	// _THURSDAY_SHORT
	{ "THURSDAY_SHORT", "四", "DO", "T", "T", "K", "四" },

	// _FRIDAY_SHORT
	{ "FRIDAY_SHORT", "五", "VR", "F", "F", "J", "五" },

	// _SATURDAY_SHORT
	{ "SATURDAY_SHORT", "六", "ZA", "S", "S", "", "六" },

	// _SUNDAY_SHORT
	{ "SUNDAY_SHORT", "天", "ZO", "S", "S", "M", "天" },

    // First B-page, B-page2, and T-page

	{	"ABORT", "取消", "STOP", "STOP", "STOP", "BERHENTI", "取消" },

	// _MORE

	{	"MORE", "加餐", "MEER", "MORE", "MEHR", "LEBIH", "加餐" },

	// _TAKEAWAY

	{	"TAKEAWAY", "打包", "AFHALEN", "TAKEAWAY", "MITNEHMEN", "KELUAR", "打包" },

	// _KITCHEN1

	{	"KITCHEN1", "厨房", "KEUKEN", "KITCHEN", "K}CHE", "DAPUR", "廚房" },

	// _KITCHEN2

	{	"KITCHEN2", "厨房2", "KEUKEN 2", "KITCHEN 2", "K}CHE 2", "DAPUR 2", "廚房2" },

	// _TABLE1

	{	"TABLE1", "台号", "TAFEL", "TABLE", "TISCH", "TABEL", "台號" },

	// _TTOTAL

	{	"TTOTAL", "总计", "TOTAAL", "TOTAL", "TOTAL", "TOTAL", "總計" },

	// _NUMBER1

	{	"NUMBER1", "号数", "NUMMER", "NUMBER", "NUMMER", "NOMOR", "號數" },

	// _OTHERS

	{	"OTHERS", "其他", "ANDERS", "CREDIT", "ANDERS", "KREDIT", "其他" },

    // _EMPLOYEE
    {   "EMPLOYEE", "同事", "MEDEWERKER", "COLLEAGUE", "KOLLEGE", "KOLEGA", "同事" },

	// _MANAGER
	{	"MANAGER", "老板", "MANAGER", "BOSS", "CHEF", "MANAJER", "老闆" },

	// _ACCOUNT_ARCHIVE
	{   "ACCOUNT_ARCHIVE", "日产存档", "DAGOPBRENGST ARCHIVEREN", "ARCHIVE DAYS", "ARCHIV TAGE",
	    "HARI ARSIP", "歸檔天" },

	// _ACCOUNTREP31
	{	"ACCOUNTREP31", "会计报告", "ACCOUNT BERICHT",
		"ACCOUNT MESSAGE", "ACCOUNT BERICHT", "PESAN ACCOUNT", "會計報告" },

	// _ACCOUNT_LONG
	{	"ACCOUNT_LONG", "会计报告", "ACCOUNT RAPPORTAGE",
		"ACCOUNT MESSAGE", "ACCOUNT BERICHT", "PESAN ACCOUNT", "會計報告" },

	// _ACCOUNT_TODAY
	{	"ACCOUNT_TODAY", "会计报告", "ACCOUNT DAGRAPPORTAGEDAAG",
		"ACCOUNT MESSAGE", "ACCOUNT BERICHT", "PESAN ACCOUNT", "會計報告" },

	// _ACCT_GROUP,

	{	"ACCT_GROUP", "[Graphics|Height:1]GROUP [Width:1|Height:2]%d = [Width:2]%1.1f%%[width:1|height:1]\n",
		"[red]BTW GROEP [Width:2]%d[Width:1] : [Width:2]%1.1f%%[Width:1]\n[Black]",
		"[red]TAX GROUP [Width:2]%d[Width:1] : [Width:2]%1.1f%%[Width:1]\n[Black]",
		"[red]MwSt. GRUPPE [Width:2]%d[Width:1] : [Width:2]%1.1f%%[Width:1]\n[Black]",
		"[red]TAX GROUP [Width:2]%d[Width:1] : [Width:2]%1.1f%%[Width:1]\n[Black]",
		"[Graphics|Height:1]GROUP [Width:1|Height:2]%d = [Width:2]%1.1f%%[Width:1|height:1]\n"
	},

	// _ACCT_TA_SUBTOTAL,

	{	"ACCT_TA_SUBTOTAL", "外卖总数 [Height:2]%s[Height:1]\n",
		"SUBTOTAAL AFHALEN    :[subtab:12]%s\n",
		"SUBTOTAL TAKEAWAY    :[subtab:12]%s\n",
		"SUBTOTAL ABHOLEN     :[subtab:12]%s\n",
		"SUBTOTAL BAWA PERGI  :[subtab:12]%s\n",
		"外賣總數 [Height:2]%s[Height:1]\n",
	},

	// _ACCT_TA_TOTAL,

	{	"ACCT_TA_TOTAL", "外卖总数 [Height:2]%s[Height:1]\n",
		"TOTAAL AFHALEN    :[subtab:12]%s\n",
		"TOTAL TAKEAWAY    :[subtab:12]%s\n",
		"TOTAL ABHOLEN     :[subtab:12]%s\n",
		"TOTAL BAWA PERGI  :[subtab:12]%s\n",
		"外賣總數 [Height:2]%s[Height:1]\n"
	},

	// _ACCT_SI_SUBTOTAL
	{	"ACCT_SI_SUBTOTAL", "餐楼总数 [Height:2]%s[Height:1]\n",
		"SUBTOTAAL RESTAURANT :[subtab:12]%s\n",
		"SUBTOTAL RESTAURANT  :[subtab:12]%s\n",
		"SUBTOTAL RESTAURANT  :[subtab:12]%s\n",
		"RESTAURANT SUBTOTAL  :[subtab:12]%s\n",
		"餐樓總數 [Height:2]%s[Height:1]\n"
	},

	// _ACCT_SI_TOTAL,

	{	"ACCT_SI_TOTAL", "餐楼总数 [Height:2]%s[Height:1]\n",
		"TOTAAL RESTAURANT :[subtab:12]%s\n",
		"TOTAL RESTAURANT  :[subtab:12]%s\n",
		"TOTAL RESTAURANT  :[subtab:12]%s\n",
		"RESTAURANT TOTAL  :[subtab:12]%s\n",
		"餐樓總數 [Height:2]%s[Height:1]\n"
	},

	// _ACCT_TA_TAX,

	{	"ACCT_TA_TAX", "外卖税[Height:2]   %s[Height:1]\n",
		"BTW AFHALEN      :[subtab:12]%s\n",
		"TAX TAKEAWAY      :[subtab:12]%s\n",
		"MwSt. ABHOLEN     :[subtab:12]%s\n",
		"PPN COLLECTION    :[Subtab:12]%s\n",
		"外賣稅[Height:2] %s[Height:1]\n"
	},

	// _ACCT_SI_TAX

	{	"ACCT_SI_TAX", "餐楼税[Height:2]   %s[Height:1]\n",
		"BTW RESTAURANT   :[subtab:12]%s\n",
		"TAX RESTAURANT    :[subtab:12]%s\n",
		"MwSt. RESTAURANT  :[subtab:12]%s\n",
		"PPN RESTAURANT    :[Subtab:12]%s\n",
		"餐樓稅[Height:2] %s[Height:1]\n"
	},

	// _ACCT_SUBTOTAL
	{	"ACCT_SUBTOTAL", "餐楼总数 [Height:2]%s[Height:1]\n",
		"SUBTOTAAL  :[subtab:12]%s\n",
		"SUBTOTAL   :[subtab:12]%s\n",
		"SUBTOTAL   :[subtab:12]%s\n",
		"RESTAURANT :[subtab:12]%s\n",
		"餐樓總數 [Height:2]%s[Height:1]\n"
	},

	// _ACCT_TAX
	{	"ACCT_TAX", "餐楼税 [Height:2]%s[Height:1]\n",
		"BTW        :[subtab:12]%s\n",
		"TAX        :[subtab:12]%s\n",
		"MwSt.      :[subtab:12]%s\n",
		"PPN        :[Subtab:12]%s\n",
		"餐樓稅 [Height:2]%s[Height:1]\n",
	},

	// _ACCT_TOTAL
	{	"ACCT_TOTAL", "餐楼总数 [Height:2]%s[Height:1]\n",
		"TOTAAL     :[subtab:12]%s\n",
		"TOTAL      :[subtab:12]%s\n",
		"TOTAL      :[subtab:12]%s\n",
		"TOTAL      :[subtab:12]%s\n",
		"餐樓總數 [Height:2]%s[Height:1]\n"
	},

	// _ACCOUNT_ITEM_KITCHEN,
	{ "ACCOUNT_ITEM_KITCHEN", "KEUKEN", "KEUKEN", "KEUKEN", "KEUKEN", "KEUKEN", "KEUKEN" },

	// _ACCOUNT_ITEM_DRINKS,
	{ "ACCOUNT_ITEM_DRINKS", "DRANKEN", "DRANKEN", "DRANKEN", "DRANKEN", "DRANKEN", "DRANKEN" },

	// _ACCOUNT_ITEM_BAR,
	{ "ACCOUNT_ITEM_BAR", "BAR", "BAR", "BAR", "BAR", "BAR", "BAR" },

	// _ACCOUNT_ITEM_OTHERS,
	{ "ACCOUNT_ITEM_OTHERS", "ANDERS", "ANDERS", "ANDERS", "ANDERS", "ANDERS", "ANDERS" },

	// _ACCOUNT_ITEM_KITCHEN2,
	{ "ACCOUNT_ITEM_KITCHEN2", "KEUKEN2", "KEUKEN2", "KEUKEN2", "KEUKEN2", "KEUKEN2", "KEUKEN2" },

	// _ACCOUNT_ITEM_SUSHI,
	{ "ACCOUNT_ITEM_SUSHI", "SUSHI", "SUSHI", "SUSHI", "SUSHI", "SUSHI", "SUSHI" },

	// _ACCOUNT_ITEM_NONFOOD,
	{ "ACCOUNT_ITEM_NONFOOD", "NONFOOD", "NONFOOD", "NONFOOD", "NONFOOD", "NONFOOD", "NONFOOD" },

	// _ACCOUNT_ITEM_KITCHEN3,
	{ "ACCOUNT_ITEM_KITCHEN3", "KEUKEN3", "KEUKEN3", "KEUKEN3", "KEUKEN3", "KEUKEN3", "KEUKEN3" },

	// _ACCOUNT_ITEM_KITCHEN4,
	{ "ACCOUNT_ITEM_KITCHEN4", "KEUKEN4", "KEUKEN4", "KEUKEN4", "KEUKEN4", "KEUKEN4", "KEUKEN4" },

	// _ACCOUNT_ITEM_KITCHEN5,
	{ "ACCOUNT_ITEM_KITCHEN5", "KEUKEN5", "KEUKEN5", "KEUKEN5", "KEUKEN5", "KEUKEN5", "KEUKEN5" },

	// _ACCOUNT_CLUSTERS

	{ "ACCOUNT_CLUSTERS", "帐户群", "BELASTING CLUSTERS", "ACCOUNT CLUSTERS", "KONTO CLUSTER", "CLUSTER AKUN", "帳戶群" },

	// _ACCOUNT_CLUSTERS_CHANGE

	{ "ACCOUNT_CLUSTERS_CHANGE", "更改帐户群", "BELASTING CLUSTERS WIJZIGEN", "CHANGE ACCOUNT CLUSTERS", "KONTO CLUSTER ANDERN", "UBAH CLUSTER AKUN", "更改帳戶群" },

	// _ACTIVE

	{	"ACTIVE", "启用", "ACTIEF", "ACTIVE", "AKTIVIERT", "AKTIF", "啟用" },

	// _ADD_FOOD

	{   "ADD_FOOD", "重复食物", "KOPIE ETEN", "COPY FOOD", "DUPLIZIER ESSEN", "MAKAN DUPLIKASI", "重複食物" },

	// _ADD_FREE_TIME

	{   "ADD_FREE_TIME", "免费时间", "GRATIS TIJD", "FREE TIME", "FREIZEIT HINZUFUGEN", "TAMBAHKAN WAKTU LUANG", "免費時間"	},

	// _ADD_PART

	{ "ADD_PART", "添加成分", "ONDERDEEL TOEVOEGEN", "ADD PART", "ZUTATEN HINZUFUGEN", "TAMBAH BAHAN", "添加成分" },

	// _ADD_STORE

	{   "ADD_STORE", "添加到库存", "VOORRAAD AANVULLEN", "ADD TO INVENTORY", "VORRAT HINZUFUGEN", "TAMBAHKAN KE STOK", "添加到庫存" },

	// _ADD_TABLE

	{
		"ADD_TABLE", "加桌子", "EXTRA TAFEL", "PLUS TABLE", "EXTRA TISCH", "MEJA TAMBAHAN", "加桌子"
 	},

	// _ADD_TAX_CLUSTER

	{ "ADD_TAX_CLUSTER", "加税组", "CLUSTER TOEVOEGEN", "ADD CLUSTER", "STEUER HINZUFUGEN", "TAMBAH CLUSTER", "加稅組" },

	// _AFTER_BILL_ADD_FOOD
	{   "AFTER_BILL_ADD_FOOD", "买单之后加餐?", "NA REKENING ETEN TOEVOEGEN?", "AFTER BILL ADD FOOD?", "NACH RECHNUNG ESSEN HINZU?", "SETELAH TAGIHAN MENAMBAHKAN MAKANAN?",
			"買單之後加餐?" },

	// _ADD_MENU_CARD
	{ "ADD_MENU_CARD", "加菜单", "MENUKAART TOEVOEGEN", "ADD MENU CARD", "MENUKARTE HINZUFUGEN", "TAMBAHKAN KARTU MENU", "加菜單" },

	// _ADD_MONEY
	{ "ADD_MONEY", "充值", "GELD TOEVOEGEN", "ADD MONEY", "GELD HINZUFUGEN", "TAMBAHKAN UANG", "充值" },

	// _ADD_TIME30
	{ "ADD_TIME30", "加30分钟", "30 MINUTEN EXTRA", "EXTRA 30 MINUTES", "ZUSATZLICHE 30 MIN", "EKSTRA 30 MENIT", "加30分鐘" },

    // _ALIGN
	{ "ALIGN", "对齐", "ALIGN", "ALIGN", "AUSRICHTEN", "MELURUSKAN", "對齊" },

	// _ALL_KEYS

	{   "ALL KEYS", "所有的钥匙", "ALLE SLEUTELS", "ALL KEYS", "ALLE SCHLÜSSEL", "SEMUA KUNCI", "所有的鑰匙" },

	// _ALREADY_PAYED
	{
		"ALREADY_PAYED", "一切都付了", "ALLES REEDS BETAALD", "ALL IS PAYED", "ALLES IST BEZAHLT", "SEMUA DIBAYARKAN", "一切都付了"
	},

	// _AMERICAN_EXPRESS

	{	"AMERICAN_EXPRESS", "AM.EXPRESS", "AM.EXPRESS", "AM.EXPRESS", "AM.EXPRESS", "AM.EXPRESS", "AM.EXPRESS" },

    // _ARTICLES

    {   "ARTICLES", "更改文章", "VERANDER ARTIKELEN", "CHANGE ARTICLES", "ARTIKEL ÄNDERN", "GANTI ARTIKEL", "更改文章" },

	// _ASAP
	{   "ASAP", "尽快", "SNEL", "ASAP", "SCHNELL", "SEPAT", "盡快" },

	// _BACK
	{	"BACK", "取消", "TERUG", "BACK", "ZURUCK", "KEMBALI", "取消" },

	// _BACKGROUND1
	{   "BACKGROUND1", "背景颜色 1", "ACHTERGROND KLEUR 1", "BACKGROUND COLOUR 1", "HINTERGRUND FARBE 1", "WARNA LATAR 2", "背景顏色 1" },

	// _BACKGROUND2
	{   "BACKGROUND2", "背景颜色 2", "ACHTERGROND KLEUR 2" , "BACKGROUND COLOUR 2", "HINTERGRUND FARBE 2", "WARNA LATAR 2", "背景顏色 2" },

	// _BACKGROUND_SELECTED
	{   "BACKGROUND_SELECTED", "背景颜色如果选定", "ACHTERGRONDKLEUR ALS GESELECTEERD",
		"BACKGROUND COLOUR IF SELECTED", "HINTERGRUNDFARBE FALLS AUSGEWAHLT",
		"WARNA LATAR YANG DIPILIH", "背景顏色如果選定" },

	// _BANK_ACCOUNT
	{   "BANK_ACCOUNT", "银行信息", "BANK REKENING", "BANK ACCOUNT", "BANK KONTO", "BANK REKENING", "銀行信息" },

	// _BAR
	{	"BAR", "酒吧", "BAR", "BAR", "BAR", "BAR", "酒吧" },

	// _BAR_PRINTER

	{	"BARP", "酒吧打印机", "BAR PRINTER", "BAR PRINTER", "BAR DRUCKER", "BAR PRINTER", "酒吧打印機" },

	// _BEZORGING

	{	"BEZORGING", "时间", "BEZORGING:", "DELIVERY:", "BESORGUNG:", "PENGIRIMAN:", "時間" },

	// _BEZORGKOSTEN

	{	"BEZORGKOSTEN", "时间价", "BEZORGKOSTEN", "DELIVERY CHARGE", "BESORGUNGSKOSTEN", "BIAYA PENGIRIMAN", "時間價" },

	// _BILL_DEFAULT_DISCOUNT
	// @todo vertaal
	{
		"BILL_DEFAULT_DISCOUNT", "REKENING KORTING", "REKENING KORTING", "REKENING KORTING", "REKENING KORTING", "REKENING KORTING", "REKENING KORTING"
	},

	// _BILL_HEADER

	{	"BILL_HEADER", "买单", "AFREKENEN", "BILL MODE", "RECHNUNG", "BON", "買單" },

	// _BILL_PAYMENTS

	{   "BILL_PAYMENTS", "付款", "BETAALWIJZE", "PAY METHOD", "BEZAHLUNG", "UANG", "付款" },

	// _BILL_TOP_RIGHT

	{	"BILL_TOP_RIGHT", "印单", "REKENING", "BILL", "RECHNUNG", "CATATAN", "印單" },

	// _BILLS_PAYED_OR_NOT

	{   "BILLS_PAYED_OR_NOT", "是否已付帐单？", "ZIJN GEMARKEERDE REKENINGEN BETAALD?", "ARE THE MARKED BILLS PAID?",
		"WERDEN MARKIERTE RECHNUNGEN BEZAHLT?", "APAKAH NOTA SUDAH DIBAYAR?", "是否已付帳單？" },

	// _BTW_LIST

	{	"BTW_LIST", "税单", "BTW LIJST", "TAX LIST", "MwSt. LISTE", "PAJAK NOMOR", "稅單" },

	// _BTW_LOW

	{	"BTW_LOW", "BTW LAAG", "BTW LAAG", "VAT LOW", "BTW LAAG", "BTW LAAG", "BTW LAAG" },

	// _BTW_HIGH

	{	"BTW_HIGH", "BTW HOOG", "BTW HOOG", "VAT HIGH", "BTW HOOG", "BTW HOOG", "BTW HOOG" },

	// _BTW_ZERO

	{	"BTW_ZERO", "ZONDER BTW", "ZONDER BTW", "VAT ZERO", "ZONDER BTW", "ZONDER BTW", "ZONDER BTW" },

	// _BTW_NUMBER

	{	"BTW_NUMBER", "[Graphics]税[Text]", "BTW NUMMER", "TAX NUMBER", "MwSt NUMMER", "NOMOR PAJAK", "[Graphics]稅[Text]"},

	// _BUY_ON_ACCOUNT

	{   "BUY_ON_ACCOUNT", "帐户购买", "OP REKENING KOPEN", "BUY ON ACCOUNT", "AUF RECHNUNG KAUFEN", "BELI DI AKUN", "帳戶購買" },

	// _BUYPRICE

	{	"BUYPRICE", "买价", "INKOOPPRIJS", "BUYING PRICE", "EINKAUFSPREIS", "HARGA PEMBELIAN", "買價" },

	// _CALCULATOR

	{	"CALCULATOR", "买价", "REKEN", "CALCULATOR", "RECHNEN", "KALK.", "買價" },

	// _CALENDAR

	{	"CALENDAR", "日历", "KALENDER", "CALENDAR", "KALENDER", "KALENDER", "日曆" },

	// _CANCEL

	{	"CANCEL", "取消", "STOP", "STOP", "STOP", "BATAL", "取消" },

	// _CANCEL_REASON
	{   "CANCEL_REASON", "取消原因", "REDEN WAAROM DE BESTELLING STOPT", "REASON WHY THE ORDER IS CANCELLED",
		"CANCEL GRUND", "ALASAN MENGAPA PESANAN DI STOP", "取消原因" },

	// _CANCEL_REASON_ADD_FOOD
	{   "CANCEL_REASON_ADD_FOOD", "结算后添加食物", "ETEN TOEVOEGEN NA AFREKENEN", "ADD FOOD AFTER BILLING", "ESSEN NACH ABRECHNUNG HINZUFUGEN", "TAMBAH MAKAN SETELAH TAGIHAN", "結算後添加食物" },

		// _CANCEL_REASON_BAD_FOOD
	{   "CANCEL_REASON_BAD_FOOD", "取消原因变质的食物", "HET ETEN SMAAKT SLECHT", "FOOD TASTE BAD", "CANCEL GRUND BAD FOOD", "MAKANAN TIDAK ENAK", "取消原因變質的食物" },

	// _CANCEL_REASON_DELIVERY_PROBLEM
	{   "CANCEL_REASON_DELIVERY_PROBLEM", "送外卖问题", "BEZORG PROBLEEM TRANSPORT",
		"PROBLEM WITH TRANSPORT", "PROBLEM MIT TRANSPORT", "MASALAH DENGAN TRANSPORTASI", "送外賣問題" },

	// _CANCEL_REASON_MOVE_TABLE
	{   "CANCEL_REASON_MOVE_TABLE", "联同其他交易", "SAMEN MET ANDERE TRANSACTIE", "JOIN WITH OTHER TRANSACTION",
		"MIT ANDEREN TRANSAKTION", "BERGABUNG DENGAN TRANSAKSI LAIN", "加入其他交易" },

    // _CANCEL_REASON_NO_MONEY
	{   "CANCEL_REASON_NO_MONEY", "客户没有钱", "KLANT HEEFT GEEN GELD", "CUSTOMER HAS NO MONEY", "KUNDE HAT KEIN GELD", "PELANGGAN TIADA UANG", "客戶沒有錢" },

	// _CANCEL_REASON_NO_STOCK
	{   "CANCEL_REASON_NO_STOCK", "无库存", "GEEN VOORRAAD OF INGREDIENTEN", "NO STOCK OR FOOD",
			  "NICHTS AUF LAGER", "TIDAK ADA STOK", "無庫存" },

	// _CANCEL_REASON_PERSONNEL
	{   "CANCEL_REASON_PERSONNEL", "食物的人员", "ETEN VOOR HET PERSONEEL", "FOOD FOR PERSONNEL", "ESSEN FUR DAS PERSONAL",
		"MAKANAN UNTUK PERSONIL", "食物的人員" },

	// _CANCEL_REASON_RUN_AWAY
	{   "CANCEL_REASON_RUN_AWAY", "客户跑掉了", "KLANT WEGGEREND ZONDER BETALEN", "CUSTOMER RAN AWAY",
		"KUNDEN LIEF WEG", "PELANGGAN PERGI TANPA BAYAR", "客戶跑掉了" },

	// _CANCEL_REASON_TECHNICAL
	{   "CANCEL_REASON_TECHNICAL", "技術原因不能準備食物", "TECHNISCHE REDEN, KAN HET ETEN NIET MAKEN",
	    "CANNOT COOK FOR TECHNICAL REASONS", "ESSEN ZUBEREITEN NICHT MOGLICH JETZT",
	    "ALASAN TEKNIS TIDAK BISA SIAPKAN MAKANAN", "技術原因不能準備食物" },

	// _CANCEL_REASON_TELEPHONE
	{   "CANCEL_REASON_TELEPHONE", "无需客户电话订购", "KLANT HAALT BESTELLING NIET OP", "TELEPHONE ORDER WITHOUT CUSTOMER",
			"TELEFON BESTELLEN OHNE KUNDEN", "PESANAN TIDAK DI AMBIL", "無需客戶電話訂購" },

	// _CANCEL_REASON_TRAINING
	{   "CANCEL_REASON_TRAINING", "交易培训", "TRAINING TRANSACTIE", "TRAINING TRANSACTION", "TRAINING TRANSACTION",
	    "TRANSAKSI PERCOBAAN", "交易培訓" },

	// _CANCEL_REASON_WRONG_DISH
	{   "CANCEL_REASON_WRONG_DISH", "错误的食物送呈", "VERKEERDE ETEN BEZORGD", "WRONG FOOD PREPARED",
		"FALSCH GELIEFERT FOOD", "SALAH PESANAN MAKANAN", "錯誤的食物送呈" },

	// _CANCEL_REASON_CUSTOMER
	{   "CANCEL_REASON_CUSTOMER", "客户取消订单", "KLANT HEEFT AFGEZEGD",
		"CUSTOMER HAS CANCELLED THE ORDER", "DER KUNDE HAT DIE BESTELLUNG STORNIERT",
		"PELANGGAN TELAH MEMBATALKAN PESANAN", "客戶取消訂單"
	},

    // _CANCEL_REASON_TOO_LATE
    {   "CANCEL_REASON_TOO_LATE", "食物送得太晚", "ETEN TE LAAT BEZORGD",
        "FOOD DELIVERED TOO LATE", "ESSEN ZU SPÄT GELIEFERT",
        "MAKANAN DIKIRIM TERLAMBAT", "食物送得太晚"
    },

	// _CLEAN_TABLE

	{   "_CLEAN_TABLE", "擦桌子", "TAFEL OPRUIMEN", "CLEAN TABLE", "TISCH SAUBER", "BERSIH MEJA", "擦桌子" },

	// _CONGDATE

	{	"CONGDATE", "从时", "VANAF DATUM", "FROM DATE", "AB DATUM", "DARI TANGGAL", "從時" },

	// _SMARTCARD

	{	"SMARTCARD", "折扣卡", "CHIPCARD", "CHIPCARD", "CHIPCARD", "CHIP KARTU", "折扣卡" },

	// _CANNOT_DELIVER
	{
	    "CANNOT_DELIVER", "无法交付，缺少 %s！！", "KAN NIET BEZORGEN. %s ONTBREEKT!!",
		                  "CANNOT DELIVER. MISSING THE %s!!",
				          "NICHT LIEFERBAR, %s FEHLT!!",
				          "TIDAK BISA KIRIM. KEHABISAN %s!!",
						  "無法交付，缺少 %s！！" },

	// _CASH

	{	"CASH", "现金", "CONTANT", "CASH", "KASSE", "KAS", "现金" },

	// _CASH_IN

	{	"CASH_IN", "收现金", "ONTVANGEN", "RECEIVED", "BEZAHLT", "MENERIMA", "收現金" },

	// _CASH_OUT

	{	"CASH_OUT", "续回", "TERUG", "EXCHANGE", "ZUR]CKGABE", "PERUBAHAN", "續回" },

	// _CAT5

	{	"CAT5", "餐名", "NAMEN", "CAT", "NAMEN", "NAMA", "餐名" },

	// _CHANGELANG

	{	"CHANGELANG", "语言", "TAAL", "LANGUAGE","SPRACHE", "BAHASA", "語言"	},

	// _CHANGE_ADDRESS
	{
		"CHANGE_ADDRESS", "改变餐馆地址", "VERANDER NAAM+ADDRESS",
		"CHANGE NAME+ADDRESS", "NAME+ADRES ANDERN", "GANTI ALAMAT+NAMA", "改變餐館地址"
	},

	// _CHANGE_ARTICLES

	{	"CHANGE_ARTICLES", "更改文章列表", "VERANDER ARTIKELEN LIJST",
		"CHANGE ARTICLE LIST", "ARTIKELLISTE ÄNDERN", "GANTI DAFTAR ARTIKEL", "更改文章列表" },

	// _CHANGE_MENU_BARCODES
	{
		"CHANGE_MENU_BARCODES", "更改条形码", "VERANDER BARCODES", "CHANGE BARCODES", "BARCODE ÄNDERN",
		"GANTI BARCODE", "更改條碼"
	},

    // _CHANGE_BILL
	{   "CHANGE_BILL", "更改帐单", "REKENING WIJZIGEN", "CHANGE BILL", "RECHNUNG ÄNDERN", "GANTI TAGIHAN", "更改帳單" },

	// _CHANGE_OTHERS
	{	"CHANGE_OTHERS", "改变...", "VERANDER SETTINGS", "CHANGE SETTINGS",
		"ANDERN EINSTELLUNGEN", "GANTI KONFIGURASI", "改變.." },

	// _CHANGE_FONT
	{
		"CHANGE_FONT", "改变FONT", "VERANDER FONTS",
		"CHANGE FONTS", "FONTS ANDERN", "GANTI FONT", "改變FONT"
	},

	// _CHANGE_MENU_ALIAS
	{
		"CHANGE_MENU_ALIAS", "变更号码", "VERANDER MENU NUMMERS", "CHANGE MENU NUMBERS", "ANDERN MENUNUMMERN",
		"NOMOR MENU MENGUBAH", "變更號碼"
	},

	// _CHANGE_MENU_ADD_ARTICLES
	{
		"CHANGE_MENU_ADD_ARTICLES", "菜单加单", "TOEVOEGEN ARTIKELEN",
		"ADD MENU ITEMS", "NEUE ARTIKELN", "MENAMBAHKAN ITEM MENU", "菜單加單"
	},

	// _CHANGE_PRINTER
	{
		"CHANGE_PRINTER", "选择打印机", "SELECTEER PRINTER",
		"SELECT PRINTER", "SELECT DRUCKER", "SELECT PRINTER", "選擇打印機"
	},

	// _CHANGE_MENU_COLOURS
	{
		"CHANGE_MENU_COLOURS", "换菜单颜色", "KLEUREN WIJZIGEN",
		"MENU COLOURS CHANGE", "MENU COLOURS CHANGE", "WARNA GANTI MENU", "換菜單顏色"
	},

	// _CHANGE_MENU_CLUSTERS
	{
		"CHANGE_MENU_CLUSTERS", "换菜单群", "CLUSTERS WIJZIGEN",
		"MENU CLUSTER CHANGE", "MENU CLUSTER ANDERN", "KELOMPOK GANTI MENU", "換菜單群"
	},

	// _CHANGE_MENUGROUPS
	{
		"CHANGE_MENU_GROUPS", "换菜单群", "GROUPEN WIJZIGEN",
		"MENU GROUP CHANGE", "MENU GRUPPE ANDERN", "KELOMPOK GANTI MENU", "換菜單群"
	},

	// _CHANGE_MENU_MOVE_ARTICLES
	{
		"CHANGE_MENU_MOVE_ARTICLES", "菜单移动", "VERPLAATS ARTIKELEN",
		"RELOCATE ITEMS", "POSITION ANDERN",
		"RELOKASI", "菜單移動"
	},

	// _CHANGE_MENU_NAMES
	{
		"CHANGE_MENU_NAMES", "换菜单名字", "NAMEN VERANDEREN",
		"MENU NAMES CHANGE", "MENU NAMEN ANDERN", "NAMA MENU GANTI", "換菜單名字"
	},

	// _CHANGE_MENU_PICTURES
	{
		"CHANGE_MENU_PICTURES", "更换图片", "FOTO VERANDEREN", "PICTURE CHANGE", "BILD ANDERN",
		"MENGUBAH GAMBAR", "更換圖片"
	},

	// _CHANGE_MENU_PRICE
	{
		"CHANGE_MENU_PRICE", "换菜单价格", "PRIJZEN VERANDEREN",
		"PRICE CHANGES", "PREISE ANDERN", "PERUBAHAN HARGA", "換菜單價格"
	},

	// _CHANGE_MENU_TAX
	{
		"CHANGE_MENU_TAX", "换菜单税", "BTW ANDERS",
		"CHANGE TAX", "TAX GRUPPE", "GANTI PAJAK GROUP", "換菜單稅"
	},

	// _CHANGE_MENU_TYPES
	{
		"CHANGE_MENU_TYPES", "换菜单类型", "TYPES VERANDEREN",
		"CHANGE MENU TYPES", "MENU TYPEN ANDERN", "GANTI PAJAK GROUP", "換菜單類型"
	},

	// _CHECK_OTHER_PRINTER
	{	"CHECK_OTHER_PRINTER", "注意其他打印机", "AFDRUK OP ANDERE PRINTER!!",
		"PRINT TO OTHER PRINTER!!",
		"DRUCKER FEHLER!", "PERIKSA PRINTER YANG LAIN!!", "注意其他打印機" },

	// _CHECK_PRINTER
	{	"CHECK_PRINTER", "注意打印机", "CONTROLEER PRINTER!!", "CHECK PRINTER!!",
		"DRUCKER FEHLER!", "PERIKSA PRINTER !!", "注意打印機" },

	// _CHANGE_PROGRAM_COLOURS
	{
		"CHANGE_PROGRAM_COLOURS", "换菜颜色", "KLEUREN WIJZIGEN",
		"COLOURS CHANGE", "FARBEN ANDERN", "WARNA GANTI", "換菜顏色"
	},

	// _CHARITY
	{   "CHARITY", "慈善机构", "GOEDE DOEL", "CHARITY", "NACHSTENLIEBE", "AMAL", "慈善機構" },

	// _CHOOSE_LIST
	{	"CHOOSE_LIST", "选择单", "KIES LIJST ARTIKELEN", "CHOOSE LIST ARTICLES",
		"COMBINATIE LISTE", "DAFTAR MEMILIH ARTIKEL", "選擇單" },

	// _CHOOSE_MANY

	{   "CHOOSE_MANY", "选择多", "KIES VEEL", "CHOOSE MANY", "ENTSCH.VIELE", "MEMILIH BANYAK", "選擇多" },

	// _CHOOSE_FOOD

	{   "CHOOSE_FOOD", "选菜单", "KIES GERECHT", "CHOOSE FOOD", "ESSEN WAHLEN", "PILIH MAKANAN", "選菜單" },

	// _CHOOSE_TWIN

	{   "CHOOSE_TWIN", "选餐双", "KIES TWIN", "CHOOSE TWIN", "TWIN WAHLEN", "PILIH KEMBAR", "選餐雙" },

	// _CITY

	{   "CITY", "城市", "STAD", "CITY", "STADT", "KOTA", "城市" },

	// _CLEAR
	{   "CLEAR", "空", "WIS", "ERASE", "LOSCHEN", "KOSONG", "空" },

	// _CLEAR_PICTURE
	{   "CLEAR_PICTURE", "空图片", "WIS PLAATJE", "ERASE PICTURE", "LOSCHEN", "GAMBAR KOSONG", "空圖片" },

	// _CLEAR_WALLLET
	{   "CLEAR_WALLET", "清空现金抽屉？输入期初余额",
			            "GELDLA LEEGMAKEN? VOER HET BEGINSALDO IN VOOR VANDAAG",
			            "EMPTY THE CASH DRAWER? ENTER THE OPENING BALANCE",
						"KASSENSCHUBLADE LEER? GEBEN SIE DEN EROFFNUNGSSALDO EIN",
						"KOSONGKAN LACI UANG TUNAI? MASSUKKAN SALDO PEMBUKAAN",
						"清空現金抽屜？輸入期初餘額" },

	// _CLIENT
	{	"CLIENT", "顾客", "KLANT", "CLIENT", "KUNDE", "PELANGGAN", "顧客" },

	// _CLIENT_AMOUNT
	{	"CLIENT_PAY", "顾客买单", "KLANT BEDRAG", "CLIENT AMOUNT", "KUNDE BETRAG", "JUMLAH PELANGGAN", "顧客買單" },

	// _CLIENT_CHOOSE
	{   "CLIENT_CHOOSE", "选择客户", "KIES KLANT", "CHOOSE CUSTOMER", "WAHLEN KUNDEN", "MEMILIH PELANGGAN", "選擇客戶" },

	// _CLIENT_LIST
	{   "CLIENT_LIST", "改变客户", "KLANT LIJST", "CUSTOMER LIST", "KUNDEN", "PELANGGAN", "改變客戶" },

	// _CLIENT_PAYS_WITH
	{
		"CLIENT_PAYS_WITH", "客户付钱", "KLANT BETAALT", "CUSTOMER PAYS", "KUNDE BEZAHLT", "MEMBAYAR", "客戶付錢"
	},

	// _CLUSTER_ADD
	{ "CLUSTER_ADD", "添加集群", "NIEUWE CLUSTER", "", "", "", "添加集群" },

	// _CLUSTER_ASK_DUPLICATE
	{ "CLUSTER_ASK_DUPLICATE", "重复的集群?", "DUPLICATE CLUSTER?", "", "", "", "重複的集群?" },

	// _CLUSTER_ASK_REMOVE
	{ "CLUSTER_ASK_REMOVE", "集群中删除?", "CLUSTER REMOVE?", "", "", "", "集群中刪除?" },

	// _CLUSTER_DUPLICATE
	{ "CLUSTER_DUPLICATE", "重复的集群", "CLUSTER DUPLICEREN", "", "", "", "重複的集群" },

	// _CLUSTER_NAME
	{ "CLUSTER_NAME", "群集名称", "CLUSTER NAAM", "CLUSTER NAME", "", "", "群集名稱" },

	// _CLUSTER_REMOVE,
	{ "CLUSTER_REMOVE", "集群中删除", "CLUSTER WEGHALEN", "", "", "", "集群中刪除" },

	// _COMPANY
	{   "COMPANY", "公司", "BEDRIJF", "COMPANY", "FIRMA", "PERUSAHAAN", "公司" },

	// _CONFIGURATION
	{	"CONFIGURATION", "功能改变", "VERANDER CONFIGURATIE", "CHANGE CONFIGURATION",
		"ANDERN EINSTELLUNGEN", "GANTI KONFIGURASI", "功能改變" },

	// _CONFIRM
	{	"CONFIRM", "定单", "ORDER MAKEN",
		"ORDER MAKEN", "ORDER MACHEN", "MEMBUAT ORDER", "定單" },

	// _CONFIRM_NOTA

	{	"CONFIRM_NOTA", "定单", "ORDER MAKEN", "FIX ORDER", "ORDER MACHEN", "FIX ORDER", "定單" },

	// _COOKING_OVERVIEW

	{ 	"COOKING_OVERVIEW", "厨房概览", "OVERZICHT KEUKEN", "OVERVIEW KITCHEN", "KÜCHENÜBERSICHT", "TINJAUAN KITCHEN", "概述廚房"},

	// _COPY_GROUP

	{   "COPY_GROUP", "复制到整个组", "KOPIE NAAR GEHELE GROUP", "COPY TO ENTIRE GROUP", "KOPIE GESAMTE GRUPPE", "COPY KE SELURUH KELOMPOK", "複製到整個組" },

	// _CREDIT

	{	"CREDIT", "减收", "CREDIT", "CREDIT", "CREDIT", "KREDIT", "減收" },

	// _CREDIT_CARD

	{   "CREDIT_CARD", "CREDIT CARD", "CREDIT CARD", "CREDIT CARD", "CREDIT CARD", "CREDIT CARD" },

	// _CURRENT_STORE

	{   "CURRENT_STORE", "存货", "VOORRAAD", "STOCK", "LAGER", "STOK", "存貨" },

	// _CUSTOMER

	{   "CUSTOMER", "顾客", "KLANT", "CUSTOMER", "KUNDE", "PELANGGAN", "顧客" },

	// _CUT_PAPER

	{   "CUT_PAPER", "切纸", "PAPIER SNIJDEN", "CUT PAPER", "PAPIER SCHNEIDEN", "POTONGAN KERTAS", "切紙",  },

	// _DAODATE

	{	"DAODATE", "到时", "TOT DATUM", "UNTIL DATE", "BIS DATUM", "SAMPAI DATE", "到時" },

	// _DATE_DELIVERED

	{	"DATE_DELIVERED", "预计交货日期", "VERWACHTE LEVERDATUM", "DATE TO DELIVER", "LIEFERDATUM", "TANGAL DIKIRIM", "預計交貨日期" },

	// _DATE_ORDERED

	{	"DATE_ORDERED", "订购日期", "DAG VAN BESTELLING", "DATE ORDERED", "BESTELLDATUM", "TANGAL PEMESANAN", "訂購日期" },

	// _DAY

	{	"DAY", "日", "DAG", "DAY", "TAG", "HARI", "日" },

	// _DAY31

	{	"DAY31", "31日", "31 DAGEN", "31 DAYS", "31 TAGE", "31 HARI", "31日" },

	// _DEBIT

	{	"DEBIT", "加收", "DEBIT", "DEBIT", "DEBIT", "DEBIT", "加收" },

	// _DELETE

	{   "DELETE", "删除", "VERWIJDEREN", "REMOVE",
			"ENTFERNEN", "MENGHAPUS", "刪除" },

	// _DELETE_FOOD

	{   "DELETE_FOOD", "删除一个项目？", "DEZE VERWIJDEREN?", "REMOVE ITEM?",
			"EIN ENTFERNTEN?", "MENGHAPUS ITEM?", "刪除的項目？" },

	// _DELETED_FOOD

	{   "DELETED_FOOD", "打印食品删除", "ETEN VERWIJDERD AFDRUK", "PRINT REMOVED FOOD",
			"DRUCK ESSEN ENTFERNT", "MAKANAN DIHAPUS", "打印食品刪除" },

	// _DELETED37BTN9, BTN1-2, BTN4, ICON1,

	{	"DELETED37", "[Graphics|Height:1]删除: 伙记 [Height:2]%d[Height:1]",
		"ORDERS WEG: OBER %d",
		"DELETED ORDERS: SLEUTEL %d", "ORDERS ENTFERNT: SCHLÜSSEL %d",
	    "DIHAPUS PESANAN: OBER %d",
	    "[Graphics|Height:1]刪除: 夥記[Height:2]%d[Height:1]" },

	// _DELIVERED

		{
			"DELIVERED", "订单已交付", "BESTELLING GELEVERD", "ORDER DELIVERED", "BESTELLUNG GELIEFERT", "ORDER DISEDIAKAN", "訂單已交付"
		},

	// _DEPOSIT

	{	"DEPOSIT", "订金", "AANBETALING",
		"DEPOSIT", "ANBEZAHLUNG", "DEPOSIT", "訂金" },

	// _DESIGN_ADVERTISEMENT

	{  "DESIGN_ADVERTISEMENT", "改变公告", "WIJZIG ADVERTENTIES",
			"CHANGE ADVERTISEMENT", "ÄNDERUNG ADVERTISEMENT",
			"IKLAN PERUBAHAN", "改變公告" },

	// _DISCOUNT

	{	"DISCOUNT", "折扣", "KORTING ", "DISCOUNT", "RABBAT  ", "DISKON  ", "折扣" }, // changed 31/10

	// _DISCOUNT21

	{	"DISCOUNT21", "折扣总数", "TOTAAL KORTING", "TOTAL DISCOUNT", "TOTAL RABBAT", "DISKON TOTAL", "折扣總數" },

	// _DOWNLOAD_CONFIGURATION

	{   "DOWNLOAD_CONFIGURATION", "DOWNLOAD CONFIGURATION", "DOWNLOAD CONFIGURATION", "DOWNLOAD CONFIGURATION",
			  "DOWNLOAD CONFIGURATION", "DOWNLOAD CONFIGURATION", "DOWNLOAD CONFIGURATION" },

	// _DOWNLOAD_MENU

	{   "DOWNLOAD_MENU", "DOWNLOAD MENU", "DOWNLOAD MENU", "DOWNLOAD MENU", "DOWNLOAD MENU", "DOWNLOAD MENU", "DOWNLOAD MENU" },

	// _DRINKS_PRINTER

	{   "DRINKS_PRINTER", "饮料打印机", "DRANKEN PRINTER", "DRINKS PRINTER", "GETRANKE DRUCKER", "MINUMLAH", "飲料打印機" },

	// _DRINKS2

	{   "DRINKS2", "饮料", "DRANKEN", "DRINKS", "GETRANKE", "MINUMLAH", "飲料" },

	// _DUPLICATE_CUSTOMER

	{   "DUPLICATE_CUSTOMER", "重复的客户", "DUPLICEER KLANT",
		"DUPLICATE CUSTOMER", "DUPLICIEREN KUNDE", "DUPLIKAT PELANGGAN", "重複的客戶" },

	// _DUPLICATE_MENU_CARD

	{	"DUPLICATE_MENU_CARD", "复制菜单", "KOPIE MENUKAART", "COPY MENU", "MENUKARTE KOPIEREN", "SALIN KARTU MENU", "複製菜單"	},

	// _EDITNAME

	{	"EDITNAME", "改餐", " EDIT NAAM", " EDIT NAME", "ANDERN NAME", "EDIT NAMA", "改餐" },

	// _EDIT_BILL_FOOTER

	{   "EDIT_BILL_FOOTER", "改变账单消息", "EDIT REKENING FOOTER", "EDIT BILL FOOTER", "ÄNDERN RECHNUNGSNACHRICHT", "UBAH PESAN PENAGIHAN", "更改結算消息" },

	// _EDIT_INVENTORY

	{ "EDIT_INVENTORY", "库存"	, "VOORRAAD BEHEER", "CHECK INVENTORY", "INVENTAR", "INVENTARIS", "庫存" },

	// _EDIT_PART

	{ "EDIT_PART", "改变成分", "ONDERDEEL WIJZIGEN", "CHANGE PART", "ZUTATEN ÄNDERN", "BAHAN PERUBAHAN", "改變成分" },

	// _EMPTY_TABLE

	{	"EMPTY_TABLE", "  空台", " LEGE TAFEL", " EMPTY TABLE", " TISCH LEER", "KOSONG", " 空台" },

	// _END_TIME

	{   "END_TIME", "停止时间", "STOPTIJD", "STOP TIME", "END ZEIT", "HENTIKAN WAKTU", "停止時間" },

	// _ENTER
	{
		"ENTER", "OK", "OK", "OK", "OK", "OK", "OK"
	},

	// _CHOOSE_TO_ORDER
	{
		"CHOOSE_TABLE_TO_ORDER", "输入单", "KIES BESTELWIJZE", "CHOOSE TO ORDER", "BESTELLEN", "PILIH PESANAN", "輸入單"
	},

	// _CONFIRM_ITEM
	{
	   "CONFIRM", "注意", "IS ALLES GOED?", "CONFIRM", "CONFIRM", "注意"
	},

	// _DATE
	{	"DATE", "日期", "DATUM", "DATE", "DATUM", "TANGGAL", "日期" },

	// _DUPLICATE ADD
	{   "DUPLICATE_ADD", "复制广告", "KOPIE ADVERTENTIE", "COPY ADVERTISEMENT", "KOPIE ADVERTISEMENT", "COPY IKLAN", "複製廣告" },

	// _EAT_INSIDE
	{   "EAT_INSIDE", "里面吃", "HIER ETEN", "EAT HERE", "ISS HIER", "MAKAN DISINI", "裡面吃" },

	// _EMAIL
	{   "EMAIL", "E-MAIL", "EMAIL", "E-MAIL", "E-MAIL", "E-MAIL", "E-MAIL" },

	// _ERASE_ALL_KEYS
	{
		"ERASE_ALL_KEYS", "擦除所有的钱", "ARCHIVEER ALLE SLEUTELS", "ARCHIVE ALL KEYS", "ALLES LOSCHEN",
		 "MENGHAPUS SEMUA KUNCI", "擦除所有的錢"
	},

	// _ERASE_FLOORPLAN
	{
		"ERASE_FLOORPLAN", "清除平面图?", "WIS PLATTEGROND?", "ERASE FLOORPLAN?", "GRUNDRISS ZU LOSCHEN?",
		 "MENGHAPUS DENAH?", "清除平面圖?"
	},

	// _ERASE_PICTURE
	{
		"ERASE_PICTURE", "删除图片", "WIS FOTO", "ERASE PICTURE", "BILD LÖSCHEN", "MENGHAPUS GAMBAR", "刪除圖片"
	},

	// _EXACTLY

	{ 	"EXACTLY", "确切", "PRECIES", "EXACT", "GENAU", "PERSIS", "精確" },

	// _EXCHANGE_MONEY,

	{	"EXCHANGE_MONEY", "底钱", "WISSELGELD", "EXCHANGE MONEY", "WECHSELGELT", "UANG EXCHANGE", "单价" },

	// _EXPORT_ACCOUNTING,

	{   "EXPORT_ACCOUNTING", "导出数据库备份", "MAAK BOEKHOUDING BACKUP", "EXPORT DATABASE BACKUP",
			                 "EXPORTEER BOEKHOUDING", "EXPORTEER BOEKHOUDING", "導出數據庫備份" },

	// _EXPORT_CONFIGURATION,
	{   "EXPORT_CONFIGURATION", "EXPORTEER CONFIGURATIE", "EXPORTEER CONFIGURATIE", "EXPORTEER CONFIGURATIE",
	    "EXPORTEER CONFIGURATIE", "EXPORTEER CONFIGURATIE", "EXPORTEER CONFIGURATIE" },

	// _EXPORT_MENU,
	{   "EXPORT_MENU", "EXPORTEER MENU", "EXPORTEER MENU", "EXPORTEER MENU",
	    "EXPORTEER MENU", "EXPORTEER MENU", "EXPORTEER MENU" },

	// _EXIT

	{	"EXIT", "停止软件", "STOP PROGRAMMA", "STOP PROGRAM", "PROGRAM STOP", "STOP PROGRAM", "停止軟件" },

	// _EXTRA

	{	"EXTRA", "改餐", "EXTRA", "EXTRA", "EXTRA", "EXTRA", "改餐" },

	// _FACTORY_MODE

	{   "FACTORY_MODE", "恢复出厂设置", "FACTORY RESET", "FACTORY RESET", "ZURÜCKSETZEN", "MENGATUR ULANG", "恢復出廠設置" },

	// _FIND_FOOD

	{   "FIND_FOOD", "寻找食物", "ZOEK GERECHT", "FIND FOOD", "SUCHE", "MENEMUKAN MAKANAN", "尋找食物" },

	// _FIND_HANDHELD

	{   "FIND_HANDHELD", "手持在哪里", "ZOEK HANDHELDS", "SEARCH HANDHELDS",
		"SUCHE HANDHELD", "HANDHELD DI MANA", "手持在哪裡"
	},

	// _FINISH_BILL

	{	"FINISH_BILL", "打印票据", "REKENING KLAAR AFDRUK ?",
		"FINISH BILL ?", "RECHNUNG FERTIG AFDRUKKEN ?", "NOTA OKE ADRUKKEN? ", "打印票據" },

	// _FINISH_ORDER

	{	"FINISH_ORDER", "打印吗", "BESTELLING OK ?",
		"FINISH ORDERING ?", "BESTELLUNG FERTIG ?", "ORDER OKE? ", "打印嗎" },

	// _FLOOR

	{   "FLOOR", "地板", "VLOER", "FLOOR", "BODEN", "LANTAI", "地板" },

	// _FLOOR_CHOOSE
	{   "FLOOR_CHOOSE", "选择地板", "WELKE VLOER?", "CHOOSE FLOOR", "WELCHE BODEN?", "MEMILIH LANTAI", "選擇地板" },

	// _FONT
	{   "FONT", "字体", "FONT", "FONT", "SCHRIFTART", "FONT", "字體" },

	// _FOOD
	{   "FOOD", "食品", "ETEN", "FOOD", "ESSEN", "MAKANAN", "食品" },

	// _FOOD_READY
	{   "FOOD_READY", "食物准备好", "ETEN IS KLAAR", "FOOD READY", "ESSEN FERTIG", "MAKANAN SUDAH", "食物準備好" },

	// _GAIBIAN

	{	"GAIBIAN", "改变", "VERANDER IN", "CHANGE INTO", "ANDERN IN", "GANTI", "改變" },

	// _GRAPH_DAYS_REPORT

	{   "GRAPH_DAYS_REPORT", "每天总数", "DAG GRAFIEK", "DAYS DIAGRAM", "TAG DIAGRAMM", "DIAGRAM HARI", "每天總數" },

	// _GRAPH_DISCOUNT

	{   "GRAPH_DISCOUNT", "折扣", "KORTING ", "DISCOUNT", "RABBAT  ", "DISKON  ", "折扣" },

	// _GRAPH_GROUPS

	{   "GRAPH_GROUPS", "1单群", "EEN GROEP", "ONE GROUP", "EINE GRUPPE", "MELAPORKAN", "1单群" },

	// _GRAPH_GROUPS_REPORT

	{   "GRAPH_GROUPS_REPORT", "图组报告", "GROEPEN GRAFIEK", "GROUP DIAGRAM", "GRUPPE DIAGRAMM",
		"KELOMPOK MELAPORKAN", "圖組報告" },

	// _GRAPH_HOURS_REPORT

	{   "GRAPH_HOURS_REPORT", "小时总数", "GRAFIEK PER UUR", "HOURS DIAGRAM", "STUNDEN DIAGRAMM", "DIAGRAM JAM", "小時總數" },

	// _GRAPH_ITEMS

	{   "GRAPH_ITEMS", "菜单总数", "MENU ITEM", "MENU ITEMS", "MENU ARTIKELEN", "MENU ITEM", "菜單總數" },

	// _GRAPH_ITEMS_REPORT

	{   "GRAPH_ITEMS_REPORT", "菜图", "GRAFIEK ARTIKELEN", "ITEM GRAPH", "ITEM GRAPH", "LAPORAN ITEM MENU", "菜圖" },

	// _GRAPH_MONTHS_REPORT

	{   "GRAPH_MONTHS_REPORT", "月总数", "MAAND GRAFIEK", "MONTHS DIAGRAM", "MONAT DIAGRAMM", "DIAGRAM BULAN", "月總數" },

	// _GRAPH_QUANTITIES

	{   "GRAPH_QUANTITIES", "数量", "HOEVEELHEID", "QUANTITY", "QUANTITAT", "KUANTITAS", "數量" },

	// _GRAPH_QUARTER_REPORT

	{   "GRAPH_QUARTER_REPORT", "季度总数", "KWARTAAL GRAFIEK", "QUARTER GRAPH", "QUARTAL DIAGRAMM", "DIAGRAM KUARTAL", "季度總數" },

	// _GRAPH_RFID_KEYS

	{	"GRAPH_RFID_KEYS", "匙", "TOTAAL SLEUTELS", "TOTAL KEYS", "PER SCHLÜSSEL", "TIAP KUNCI", "匙" },

	// _GRAPH_TYPE

	{ "GRAPH_TYPE", "图形类型", "GRAFIEK TYPE", "GRAPHIC TYPE", "GRAFIKTYP", "JENIS GRAFIS", "圖形類型" },

	// _GRAPH_TOTALS

	{   "GRAPH_TOTALS", "总数", "TOTAAL HOOG+LAAG", "TOTAL TAX GROUPS", "TOTAL", "JUMLAH", "總數" },

	// _GRAPH_WEEKS_REPORT

	{   "GRAPH_WEEKS_REPORT", "星期总数", "WEEK GRAFIEK", "WEEK DIAGRAM", "WOCHE DIAGRAMM", "DIAGRAM MINGGU", "星期總數" },

	// _GRAPH_YEAR_REPORT

	{   "GRAPH_YEAR_REPORT", "年总数", "JAAR GRAFIEK", "YEAR GRAPH", "JAHR DIAGRAMM", "DIAGRAM TAHUN", "年總數" },

	// _GROUP

	{	"GROUP", "类    %u", "GROEP  %u", "GROUP  %u", "GRUPPE %u", "KELOMPOK %u", "類%u" },

	// _GROUP5

	{	"GROUP5", "类", "GROEP", "GROUP", "GRUPPE", "KELOMPOK", "類" },

	// _GROUP16

	{	"GROUP16", "[Graphics]类 %d: %8.2f", "GROEP %d: %8.2f",
		"GROUP %d: %8.2f", "GRUPPE %d: %8.2f", "KELOMPOK %d: %8.2f", "[Graphics]類%d: %8.2f" },

	// _HANDWRITING
	{
		"HANDWRITING", "手写", "HAND", "HAND", "HAND", "TANGAN", "手寫"
	},

	// _HEIGHT
	{
		"HEIGHT", "高度", "HOOGTE", "HEIGHT", "HÖHE", "TINGGI", "高度"
	},

	// _HELP_COLOUR_DIALOG
	{ "HELP_COLOUR_DIALOG",
	  "1.选择一些,2.选择文本或背景,3.选择颜色",
	  "1. KIES ARTIKEL,2. KIES TEXT OF ACHTERGROND,3. KIES KLEUR", "1. CHOOSE ARTICLE,2. CHOOSE TEXT/BACKGROUND, 3. CHOOSE COLOUR",
	  "1. WAHLEN SIE ETWAS,2. WAHLEN SIE TEXT ODER HINTERGRUND,3. WAHLEN SIE DIE FARBE",
	  "1. PILIH SESUATU,2. PILIH TEKS ATAU LATAR BELAKANG PILIH WARNA",
	  "1.選擇一些,2.選擇文本或背景,3.選擇顏色"
	},

    // _HORIZONTAL_SCROLL
    {
		  "HORIZONTAL_SCROLL", "水平滑动", "SCROLL\nHORIZONTAAL", "HORIZONTAL\nSCROLL",
		  "WISCHEN\nHORIZONTAL", "GESER SECARA\nHORIZONTAL", "水平滑動"
	},

	// _HOUR
	{ "HOUR", "钟头", "UUR", "HOUR", "UHR", "JAM", "鐘頭" },

	// _HOWMANYITEMS

	{	"HOWMANYITEMS", "多少个项目 %d", "HOEVEEL  %d KEUZES",
		"NR OF  %d CHOICES", "ANZAHL   %d SEITE", "TIDAK ATAU %d, PILIHAN", "多少個項目%d" },

	// _ICON
	{   "ICON", "图标", "IKOON", "ICON", "SYMBOL", "IKON", "圖標" },

	// _IDEAL

	{   "IDEAL", "IDEAL", "IDEAL", "IDEAL", "IDEAL", "IDEAL", "IDEAL" },

	// _IMPORT_CONFIGURATION

	{
		"IMPORT_CONFIGURATION", "IMPORTEER CONFIGURATIE", "IMPORTEER CONFIGURATIE",
		"IMPORTEER CONFIGURATIE", "IMPORTEER CONFIGURATIE", "IMPORTEER CONFIGURATIE",
		"IMPORTEER CONFIGURATIE"
	},

	// _IMPORT_MENU

	{ "IMPORT_MENU", "IMPORTEER MENU", "IMPORTEER MENU","IMPORTEER MENU",
			"IMPORTEER MENU","IMPORTEER MENU","IMPORTEER MENU" },

	// _IMPORT_SOFTWARE

	{ "IMPORT_SOFTWARE", "IMPORTEER SOFTWARE", "IMPORTEER SOFTWARE",
	  "IMPORTEER SOFTWARE", "IMPORTEER SOFTWARE", "IMPORTEER SOFTWARE", "IMPORTEER SOFTWARE" },

	// _IN_ORDER

	{ "IN_ORDER", "在做", "IN BESTELLING", "ORDERED COMING SOON", "AUF BESTELLUNG", "DALAM PESANAN", "在做" },

	// _INCLUDED8

	{	"INCLUDED8", "INCLUDED", "INCLUSIEF", "INCLUDED", "EINSCHL.", "TERMASUK", "INCLUDED" },

	// _INFO_ADDRESS_PC
	{   "INFO_ADDRESS_PC", "街道或邮政编码", "STRAAT OF POSTCODE", "STREET OR POSTAL STAMP",
	    "STRASSE ODER PC", "JALAN ATAU POSTCODE", "街道或郵政編碼" },

	// _INFO_MENU_CARD
	{ "INFO_MENU_CARD", "信息菜单卡", "INFORMATIE OVER DE MENUKAART", "INFO MENU CARD",
	  "INFO MENU KARTE", "KARTU MENU INFO", "資訊選單卡"},

	// _INSERT_CUSTOMER

	{   "INSERT_CUSTOMER", "加客户", "NIEUWE KLANT", "NEW CLIENT", "NEUE KUNDE", "PELANGGANG BARU", "加客戶" },

	// _INSERT_FOOD

	{   "INSERT_FOOD", "加食物", "ETEN TOEVOEGEN", "ADD NEW DISH", "MEHR ESSEN", "MENAMBAHKAN MAKANAN", "加食物" },

	// _INTERNET

	{   "INTERNET", "互联网", "INTERNET", "INTERNET", "INTERNET", "INTERNET", "互聯網" },

	// _INVOICE

	{   "INVOICE", "添加收据", "BONNETJE TOEVOEGEN", "ADD RECEIPT", "QUITTUNG HINZUFÜGEN", "TAMBAHKAN PENERIMAAN", "添加收據" },

    // ITEM_ALL_KITCHEN
    { "ALLE KEUKENS", "ALLE KEUKENS", "ALLE KEUKENS", "ALLE KEUKENS", "ALLE KEUKENS", "ALLE KEUKENS", "ALLE KEUKENS" },

    // ITEM_BAR
    { "ITEM_BAR", "BAR", "BAR", "BAR", "BAR", "BAR", "BAR" },

    // ITEM_DRINKS
    { "ITEM_DRINKS", "DRANK", "DRANK", "DRANK", "DRANK", "DRANK", "DRANK" },

    // ITEM_KITCHEN
    { "ITEM_KITCHEN1", "厨房", "KEUKEN", "KITCHEN", "KUCHE", "DAPUR", "廚房" },

    // ITEM_KITCHEN2
    { "ITEM_KITCHEN2", "厨房 2", "KEUKEN 2", "KITCHEN 2", "KUCHE 2", "DAPUR 2", "廚房2" },

    // ITEM_KITCHEN3
    { "ITEM_KITCHEN3", "厨房 3", "KEUKEN 3", "KITCHEN 3", "KUCHE 3", "DAPUR 3", "廚房3" },

    // ITEM_KITCHEN4
    { "ITEM_KITCHEN4", "厨房 4", "KEUKEN 4", "KITCHEN 4", "KUCHE 4", "DAPUR 4", "廚房4" },

    // ITEM_KITCHEN5
    { "ITEM_KITCHEN5", "厨房 5", "KEUKEN 5", "KITCHEN 5", "KUCHE 5", "DAPUR 5", "廚房5" },

    // ITEM_NONFOOD
    { "ITEM_NONFOOD", "BAR", "BAR", "BAR", "BAR", "BAR", "BAR" },

    // ITEM_OTHERS
    { "ITEM_OTHERS", "ANDERS", "ANDERS", "ANDERS", "ANDERS", "ANDERS", "ANDERS" },

    // ITEM_SUSHI
    { "ITEM_SUSHI", "SUSHI", "SUSHI", "SUSHI", "SUSHI", "SUSHI", "SUSHI" },

	// _KEY_INFO

	{	"KEY_INFO", "伙记结数", "SLEUTEL INFO","KEY INFO", "OBER INFO", "OBER INFO", "夥記結數" },

	// _KITCHEN

	{	"KITCHEN", "厨房打印机", "KEUKEN", "KITCHEN", "KUCHE", "DAPUR", "廚房打印機" },

	// _KITCHEN01

	{	"KITCHEN01", "厨房 1", "KEUKEN 1", "KITCHEN 1", "KUCHE 1", "DAPUR 1", "廚房1" },

	// _KITCHEN02

	{	"KITCHEN02", "厨房 2", "KEUKEN 2", "KITCHEN 2", "KUCHE 2", "DAPUR 2", "廚房2" },

	// _KITCHEN03

	{	"KITCHEN03", "厨房 3", "KEUKEN 3", "KITCHEN 3", "KUCHE 3", "DAPUR 3", "廚房3" },

	// _KITCHEN04

	{	"KITCHEN04", "厨房 4", "KEUKEN 4", "KITCHEN 4", "KUCHE 4", "DAPUR 4", "廚房4" },

	// _KITCHEN05

	{	"KITCHEN05", "厨房 5", "KEUKEN 5", "KITCHEN 5", "KUCHE 5", "DAPUR 5", "廚房5" },

	// _KITCHEN_COLLECT

	{	"KITCHEN_COLLECT", "厨房总数", "KEUKEN ALLES", "KITCHEN ALL", "KUCHE ALLES", "DAPUR TOTAL", "廚房總數" },

	// _KITCHEN_INDEX,

	{	"KITCHEN_INDEX", "打印厨编号", "AFDRUK KEUKEN INDEX", "PRINT KITCHEN SUB NUMBER", "DRUCK KüCHE TEILNUMMER", "DAPUR TOTAL", "打印厨编号" },

	// _KOMMA

	{	"KOMMA", "点", "KOMMA", "COMMA", "KOMMA", "COMMA", "點" },

	// _LENGTH_TABLE

	{   "LENGTH_TABLE", "长度", "TAFEL LENGTE", "TABLE LENGTH", "TISCH LANGE", "MEJA PANJANG", "長度" },

	// _LIMIT
	{   "LIMIT", "最高金额", "LIMIET", "LIMIT MONEY", "HOCHST BETRAG", "JUMLAH MAKSIMUM", "最高金額" },

	// _LIST
	{	"LIST", "买主 ", "KLANT ", "CLIENT ", "KUNDE ", "KLIEN ", "買主" },

	// _LIST42
	{	"LIST42", "[Graphics]买主 [Height:2]%s[Height:1]\n",
		"LIJST SLEUTEL %s's TRANSAKTIES\n",
		"LIST KEY %s's TRANSACTIONS\n",
		"LISTE SCHLÜSSEL %s's TRANSACTION\n",
		"DAFTAR TRANSAKSI KUNCI %s's \n",
		"[Graphics]買主 [Height:2]%s[Height:1]\n" },

	// _LOCK
	{	"LOCK", "锁机", "SLOT", "LOCK", "LOCK", "LOCK", "鎖機" },

	// _LOGIN
	{   "LOGIN", "登录", "INLOGGEN", "LOGIN", "ANMELDUNG", "GABUNG", "登錄" },

    // _LOGOFF
    {   "LOGOFF", "注销", "UITLOGGEN", "LOGOFF", "AUSLOGGEN", "LELUAR", "註銷" },

	// _MARK_BILLS_SENT

	{
		"MARK_BILLS_SENT", "账单发送给客户", "FACTUUR VERZONDEN NAAR KLANT", "", "RECHNUNG GESENDET", "TAGIHAN DIKIRIM KE PELANGGAN", "賬單發送給客戶"	},

	// _MASTERCARD

	{	"MASTERCARD", "MASTERCARD", "MASTERCARD", "MASTERCARD" , "MASTERCARD", "MASTERCARD", "MASTERCARD" },

	// _MEENEMEN

	{	"MEENEMEN", "拿", "MEENEMEN", "TAKE", "MITNEHMEN", "MENGAMBIL", "拿" },

	// _MENULIST

	{	"MENULIST", "改变菜单", "VERANDER MENU KAART", "CHANGE YOUR MENU CARD", "MENU LISTE ANDERN", "DAFTAR MENU", "改變菜單" },

	// _MENU_PRICE

	{   "MENU_PRICE", "菜单价格", "MENU PRIJS", "MENU PRICE", "MENU PREIS", "HARGA MENU", "菜單價格" },

	// _MERGE_TABLES

	{   "MERGE_TABLES", "合并两个订单?", "TAFELS SAMENVOEGEN?", "MERGE TWO TABLES?", "ZUSAMMEN FUHREN?", "BERGABUNG DUA TABEL?", "合併兩個訂單?" },

	// _MINUTES
		{ "MINUTES", "分钟", "MINUTEN", "MINUTS", "MINUTEN", "MENIT", "分鐘" },

	// _MIN10

	{	"MIN10", "+%d 分", "\t+%3d MIN\n", "\t+%3d MIN\n", "\t+%3d MIN\n", "\t+%3d MENIT\n", "+%d 分" },

	// _MOBILE
	{   "MOBILE", "手机", "MOBIEL", "MOBILE", "MOBIEL", "MOBIL", "手機" },

	// _MONTH

	{	"MONTH", "月", "MAAND", "MONTH", "MONAT", "BULAN", "月" },

	// _MONTH12

	{	"MONTH12", "12月", "12 MAANDEN", "12 MONTHS", "12 MONAT", "12 BULAN", "12月"},

	// _MONTH22

	{	"MONTH22", "月 : %s\n[Line:16]天[subtab:33]\n[Line:16]总\n",
		"[Text|Font:1]MAAND BERICHT : %s %d\n[Line:24]\n[text|Font:1]DAG[subtab:6]TOTAAL\n[Line:16|text]",
		"[Text|Font:1]MONTH ACCOUNT : %s %d\n[Line:24]\n[text|Font:1]DAY[subtab:5]TOTAL\n[Line:16|text]",
		"[Text|Font:1]MONAT BERICHT : %s %d\n[Line:24]\n[text|Font:1]TAG[subtab:6]TOTAL\n[Line:16|text]",
		"[Text|Font:1]BULAN PEMBER. : %s %d\n[Line:24]\n[text|Font:1]HARI[subtab:6]TOTAL\n[Line:16|text]",
		"月: %s\n[Line:16]天[subtab:33]\n[Line:16]總\n" },

	// _MOVE_OTHER_TABLE
	{
		"MOVE_OTHER_TABLE", "分单移动", "KIES TAFEL OPSPLITSEN",
		"CHOOSE TABLE BREAK UP", "TISCH SPALTUNG",
		"PILI MEJA PEMISAHAN", "分單移動"
	},

	// _MOVE_OTHER_TABLE1
	{
		"MOVE_OTHER_TABLE1", "另一个桌", "ANDERE TAFEL",
		"TO OTHER TABLE", "ANDEREN TISCH",
		"PNDAHKAN MEJA", "另一個桌"
	},

	// _MOVE_GROUP
	{
		"MOVE_GROUP", "移动页面", "PAGINA VERPLAATSEN",
		"MOVE PAGE", "SEITE VERSCHIEBEN",
		"PINDAH HALAMAN", "移動頁面"
	},

	// _MOVE_ITEM
	{
		"MOVE_ITEM", "移动文章", "ARTIKEL\nVERPLAATSEN",
		"CHOOSE\nITEM GROUP", "ARTIKEL\nVERSCHIEBEN",
		"PINDAH\nARTIKEL", "移動文章"
	},

	// _MOVE_TO
	{ "MOVE_TO", "订单 %s 移动到 %d", "TAFEL %s NU NAAR SLEUTEL %d", "TABLE %s MOVES TO KEY %d",
	  "TISCH % NACH SCHLÜSSEL %d", "%s UNTUK KUNCI %d", "訂單%s 移動到%d"
	},

	// _NAME
	{	"NAME", "姓名", "NAAM", "NAME", "NAME", "NAMA", "姓名" },

	// _NAME34

	{	"NAME34", "[Height:1|Graphics|6]名[Subtab:32]数量",
		"NAAM[Subtab:11]HOEVEELHEID",
		"NAME[Subtab:8]QUANTITY", "NAME[Subtab:9]QUANTIT[T"
		"NAMA[Subtab:8]QUANTITY", "NAMA[Subtab:9]KUANTITAS",
		"[Height:1|Graphics|6]名[Subtab:32]數量"
	},

	// _NAMEQUANT34

	{	"NAMEQUANT34", "[Graphics]名[Subtab:32]数量\n",
		"NAAM[Subtab:11]HOEVEELHEID\n",
		"NAME[Subtab:8]QUANTITY", "NAME[Subtab:9]QUANTIT[T",
		"NAMA[Subtab:9]KUANTITAS",
		"[Graphics]名[Subtab:32]數量\n"
	},

	// _NEW_ADD

	{	"NEW_ADD", "输入", "NIEUWE ADVERTENTIE", "NEW ADVERTISEMENT", "NEUE ANZEIGE",
			"IKLAN BARU", "輸入" },

	// _NEW_CUSTOMER

	{	"NEW_CUSTOMER", "输入", "NIEUWE AFSPRAAK", "NEW RESERVATION", "NEUE RESERVIERUNG",
			"PEMESANAN BARU", "輸入" },

    // _NEW_ITEM

    {   "NEW_ITEM", "新品", "NIEUW ARTIKEL", "NEW ARTICLE", "NEUER ARTIKEL",
            "ARTIKEL BARU", "新品" },

	// _NEXT

	{	"NEXT", "输入", "ENTER", "ENTER", "ENTER", "輸入" },

	// _NEXT_ADD

	{	"NEXT_ADD", "下一个广告", "VOLGENDE ADVERTENTIE", "NEXT ADVERTISEMENT",
		"NÄCHSTE ANZEIGE", "IKLAN BERIKUTNYA", "下一個廣告" },

	// _NEW_KEY

	{	"NEW_KEY", "请把钥匙圈", "GEEF SLEUTEL", "PLEASE PUT KEYRING",
		"GIB SCHLÜSSEL.", "MOHON PUT KEYRING", "請把鑰匙圈" },

	// _NEXT_ARTICLE

	{	"NEXT_ARTICLE", "下个", "VOLGENDE ARTIKEL",
			"NEXT ARTICLE", "NACHSTE ITEM", "PASAL NEXT", "下個" },

	// _NEXT_SLIP_PAGE

	{	"NEXT_SLIP_PAGE", "  下条匙", "GEEF VOLGENDE.",
			"INSERT NEXT", "FOLGENDE BITTE", "BERIKAN, NEXT", " 下條匙" },

	// _NON_CASH

	{	"NON_CASH", "非现金", "PIN CREDIT CARD", "NON-CASH",
			"KREDIET-KARTE", "PIN KARTU KREDIT", "非現金" },

	// _NONFOOD

	{ "NONFOOD", "非食品", "NONFOOD", "NONFOOD", "NONFOOD", "NONFOOD", "非食品" },

	// _NOP

	{ "NOP", "", "", "", "", "", "" },

	// _NOT_PAYED_YET

	{ "NOT_PAYED_YET", "尚未支付", "NOG NIET BETAALD", "NOT PAYED YET", "NOCH NICHT BEZAHLT", "BELUM DIBAYAR", "尚未支付" },

	// _NUMBER_ORDER
	  { "NUMB", "号码或名称", "BESTELLEN OP NUMMER",
	    "ORDER BY NUMBER", "ORDER BEI NUMMER",
	    "ORDER NOMOR", "號碼或名稱"
	  },

	// _NUMBER_PERSONS

	{ "NUMBER_PERSONS", "人数", "MAX PERSONEN", "MAX PERSONS", "PERSONEN", "MAX ORANG", "人數" },

	// _NUMBER_TABLES

	{ "NUMBER_TABLES", "餐桌数量", "AANTAL TAFELS", "NUMBER OF TABLES", "ANZAHL ESSTISCHE", "JUMLA MEJA", "餐桌數量" },

	// _OFFER

	{	"OFFER", "打印报价", "OFFERTE", "SPECIAL", "OFFERTE", "PENAWARAN", "打印報價" },

	// _OK

	{	"OK", "好 !", "OK !", "OK !", "GUT !", "OK!", "好!" },

	// _OLD_TRANSACTION_JUST_BILL

	{ "OLD_TRANSACTION_JUST_BILL", "这是一次旧交易，你现在需要关闭订单", "DIT IS EEN OUDE REKENING, DE REKENING MOET NU GEMAAKT WORDEN",
			 "THIS IS AN OLD TRANSACTION, YOU NEED TO PROCESS THE BILL NOW", "DIES ISTT EINE ALTE RECHNUNG, SIE SOLTEN JETZT DIE BESTELLUNG SCHLIEßEN",
			 "INI ADALAH TRANSAKSI LAMA, ANDA PERLU TUTUP ORDER SEKARANG", "這是一次舊交易，你現在需要處理這項法案" },

	// _ON_THE_ROAD

	{ "ON_THE_ROAD", "在交货", "BESTELLING ONDERWEG", "IN DELIVERY", "IN LIEFERUNG", "DI PENGIRIMAN", "在交貨" },

	// _OPTIONS

	{   "OPTIONS", "??", "OPTIES", "FILTER", "??", "??", "PILIH" },

	// _ORDER

	{
		"ORDER", "入单", "BESTELLEN", "ORDER", "ORDER", "ORDER", "餐類入單"
	},

	// _ORDER_ASSEMBLE

	{	"ORDER_ASSEMBLE", "拼凑", "SAMENSTELLEN", "ASSEMBLING", "ASSEMBLIEREN", "MEMBANGUN", "拼湊" },

	// _ORDER_CANCEL

	{	"ORDER_CANCEL", "取消订单", "STOP BESTELLING", "CANCEL ORDER", "BESTELLUNG ANNULIEREN", "DIBATALKAN ORDER", "取消訂單" },

	// _ORDER_DATE

	{	"ORDER_DATE", "订单日期", "BESTEL DATUM", "ORDER DATE", "BESTELLDATUM", "TANGGAL PEMESANAN", "訂單日期" },

	// _ORDER_DEPOSIT

	{	"ORDER_DEPOSIT", "存款金额", "AANBETAALD", "DEPOSIT", "ANBEZAHLT", "UANG MUKA", "存款金額" },

	// _ORDER_FIRST

	{   "ORDER_FIRST", "请先订购", "EERST BESTELLEN GRAAG", "FIRST ORDER PLEASE", "BESTELLEN SIE BITTE ZUERST", "PESAN DULU", "請先訂購" },

	// _ORDER_GETTAKEAWAY

	{	"ORDER_GETTAKEAWAY", "打包", "GELEVERD", "DELIVERED", "GELIEFERT", "GELEVERD", "打包"},

	// _ORDER_TAKEAWAY

	{	"ORDER_TAKEAWAY", "打包", "KLAAR VOOR LEVERING", "STANDBY DELIVER",
		"BEREIT F]R LIEFERUNG", "STANDBY MEMBERIKAN", "打包" },

	// _ORDER_TELEPHONE

	{	"ORDER_TELEPHONE", "", "INFORMEER KLANT", "INFORM CUSTOMER", "INFORMIER KUNDE", "INFORM PELANGGAN"},

	// _ORDER_WAIT

	{	"ORDER_WAIT", "", "ONDERDELEN ONDERWEG", "PARTS DUE TO ARRIVE", "BAUTEILEN UNTERWEG", "PARTS KARENA TIBA"},

	// _OTHER_BILL

	{	"OTHER_BILL", "另外单", "ANDERE REKENING", "OTHER BILL", "ANDERE RECHNUNG", "ACCOUNT LAINNYA", "另外單"},

	// _OTHER

	{ "OTHER", "另外打印机", "ANDERE PRINTER", "OTHER PRINTER", "PRINT ANDERS", "LAINNYA PRINTER", "另外打印機" },

	// _OUT6

	{	"OUT6", "完", ", UIT", ", OUT", ", AUS", "OUT", "完" },

	// _OUT8

	{	"OUT8", "  完", " UIT    ", " OUT    ", " AUS    ", "UIT    ", "完" },

	// _OVERWRITE

	{   "OVERWRITE", "传输文件", "FILE OVERSCHRIJVEN:", "OVERWRITE FILE:",
		"DATEI UBERSCHREIBEN:", "TIMPA FILE", "覆蓋文件" },

	// _PAID
	{
		"PAID", "一切都付了", "AL BETAALD", "PAID", "BEZAHLT", "DIBAYARKAN", "一切都付了"
	},

	// _PRINTPAGE

	{	"PRINTPAGE", "印页码", "PRINT PAGINA", "PRINT PAGE", "SEITE DR]CKEN", "PRINT PAGE", "印頁碼" },

	// _PAPER_SP200

	{	"PAPER_SP200", "取出纸张", "VERWIJDER PAPIER",
		"PLEASE REMOVE PAPER", "ENTFERN PAPIER", "TOLONG, HAPUS KERTAS", "取出紙張" },

	// _PAPER_TM290

	{	"PAPER_TM290", "  入纸!", " PAPIER TM290!", " PAPER TM290!", " PAPIER TM290!", "KERTAS TM290", " 入紙!" },

	// _PART_NAME

	{ "PART_NAME", "名称", "ONDERDEEL", "PART", "TEIL", "BAGAIN", "名稱" },

	// _PARTIAL_BILL

	{ "PARTIAL_BILL", "分开付款", "SPLITS", "SPLIT", "SEP.", "SEBAGIAN", "分開付款"	},

	// _PASSWORD

	{  "PASSWORD", "密码", "PASWOORD", "PASSWORD", "PASSWORT", "KATA SANDI", "密碼" },

	// _PAY

	{   "PAY", "埋单", "BETALEN", "PAY", "BEZAHLEN", "BAYAR", "埋單" },

	// _PAY_DEBIT

	{   "PAY_DEBIT", "DEBIT", "DEBIT", "DEBIT", "DEBIT", "DEBIT", "DEBIT" },

	// _PAY_ZETTLE,

	{   "PAY_ZETTLE", "ZETTLE", "ZETTLE", "ZETTLE", "ZETTLE", "ZETTLE", "ZETTLE" },

	// _PAYED

	{   "PAYED", "已付账单", "BETAALD", "BEZAHLT", "PAYED", "DIBAYAR", "已付賬單" },

	// _PAYED_TRANSACTIONS

	{   "PAYED_TRANSACTIONS", "已付账单", "REKENINGEN BETAALD", "RECHNUNGEN BEZAHLT", "BILLS PAYED", "BILLS DIBAYAR", "已付賬單" },

	// _PAYMENT

	{   "PAYMENT", "新付款", "BETALING", "ADD PAYMENT", "BEZAHLUNG", "PEMBAYARAN", "新付款" },

	// _PAYPAL

	{   "PAYPAL", "PAYPAL", "PAYPAL", "PAYPAL", "PAYPAL", "PAYPAL", "PAYPAL" },

	// _PAYS_WITH

	{   "PAYS_WITH", "使用支付", "BETAAL MET", "PAY WITH", "BEZAHLEN MIT", "BAYAR MENGGUNAKAN", "使用支付" },

	// _PER_DAY

	{   "PER_DAY", "??", "PER DAG", "??", "??", "??", "??" },

	// _PER_MONTH

	{   "PER_MONTH", "??", "PER MAAND", "??", "??", "??", "??" },

	// _PER_YEAR

	{   "PER_YEAR", "??", "PER JAAR", "??", "??", "??", "??" },

	// _PERCENTAGE

	{   "PERCENTAGE", "百分比", "PERCENTAGE", "PERCENT", "PROZENT", "PERCENTASE", "百分比" },

	// _PERSON

	{   "PERSON", "人", "PERSOON", "PERSON", "MENSCH", "ORANG", "人" },

	// _PICTURE

	{	"PICTURE", "图片", "FOTO", "PICTURE", "BILD", "GAMBAR", "圖片" },

	// _PIN

	{   "PIN", "PIN", "PIN", "PIN", "PIN", "PIN", "PIN" },

	// _PINYsIN

	{   "PINYIN", "拼音", "PINYIN", "PINYIN", "PINYIN", "PINYIN", "拼音" },

	// _PLACE_ORDER

	{   "PLACE_ORDER", "订购部分", "DEEL BESTELLEN", "ORDER PART", "TEIL BESTELLUNG", "MEMESAN", "訂購部分" },

	// _PORTION

	{   "PORTION", "一部分", "PORTIE KLEIN", "PORTION", "TEIL", "PORSI", "一部分" },

	// _PREVIOUS_ADD

	{   "PREVIOUS_ADD", "以前的广告", "VORIGE ADVERTENTIE",
		"PREVIOUS ADVERTISEMENT", "VORHERIGE ANZEIGE", "IKLAN SEBELUMNYA", "以前的廣告" },

	// _PRICE_CHANGE
	{
		"PRICE_CHANGE", "变动价格", "PRIJS VERANDEREN", "PRICE CHANGE", "PREISANDERUNG", "PERUBAHAN HARGA", "變動價格"
	},

	// _PRINT_AGAIN,
	{
		"PRINT_AGAIN", "打印", "AFDRUK OPNIEUW", "PRINT AGAIN", "DRUCK NOCHMALS", "PRINTER LAGI", "打印"
	},

	// _PRINT_COLLECT,

	{	"PRINT_COLLECT", "打印收集",
		"PRINT VERZAMELING", "PRINT COLLECTION", "SAMMLUNG DRUCKER", "PRINTER KOLEKSI", "打印收集" },

	// _PRINT_FOOD

	{	"PRINT_FOOD", "打印食物", "ETEN AFDRUKKEN", "PRINT FOOD", "ESSEN DRUCKEN", "CETAK MAKANAN", "打印食物" },

	// _PRINT_KITCHEN

	{	"PRINT_KITCHEN", "厨房打印机",
		"PRINTEN IN KEUKEN", "PRINT IN KITCHEN", "ABDR�CK IN KUCHE", "PRINT DI DAPUR", "廚房打印機" },

	// _PRINT_KITCHEN_LOCAL

	{	"PRINT_KITCHEN_LOCAL", "打印厨房荷兰",
		"ALLE TALEN IN KEUKEN", "PRINT ALl LANGUAGES IN KITCHEN", "ABDR�CK ALLES IN KUCHE", "PRINT NL DI DAPUR", "打印廚房荷蘭" },

	// _PRINT_OFFER

	{ "PRINT_OFFER", "打印报价?", "OFFERTE AFDRUKKEN?", "PRINT OFFER?", "OFFERTE DRUCKEN?", "MAU PENAWARAN?", "打印報價?" },

	// _PRINT_KITCHEN2

	{ "PRINT_KITCHEN2", "打印厨房",
	  "PRINTEN IN KEUKEN", "PRINT IN KITCHEN", "ABDR�CK IN KUCHE", "PRINT DI DAPUR", "打印廚房"	},

	// _PRINT_LIST

	{	"PRINT_LIST", "打印清单", "AFDRUK LIJST", "PRINT LIST", "LISTE DRUCKEN", "DAFTAR CETAK", "打印清單" },

	// _PRINT_PRINTERS

	{	"PRINT_PRINTERS", "打印机和税务组", "AFDRUK PER PRINTER", "PRINTERS & TAX GROUPS",
			              "DRUCKER UND STEUERGRUPPE", "PRINTER DAN GRUP PAJAK", "打印機和稅務組" },

	// _PRINT_SLIP

	{	"PRINT_SLIP", "打印单", "SLIP PRINTER", "SLIP BILL", "SLIP DRUCKER", "PRINTER SLIP", "打印單" }, // 滑动打印机

	// _PRINT_ROLL

	{	"PRINT_ROLL", "卷打印机", "REKENING ROL PRINTER" , "PRINT ROLL BILL", "ROLL DRUCKER", "PRINTER PERAN NOTA", "卷打印機" }, // 卷打印机

	// _PRIJS_PP

	{	"PRIJS_PP", "价格", "PRIJS", "PRICE", "PREIS", "HARGA", "價格" },

	// _PRINTER

	{	"PRINTER", "打印机", "AFDRUK", "PRINTER", "DRUCKER", "PRINTER", "打印機" },

	// _PRINTER_DETAILS

	{	"PRINTER_DETAILS", "打印机配置", "AFDRUK DETAILS", "PRINTER CONFIGURATION", "DRUCKERKONFIGURATION", "KONFIGURASI PRINTER", "打印機配置" },

	// _PRINTER_NETWORK_PROBLEM

	{   "PRINTER_NETWORK_PROBLEM", "打印机不工作,,检查您的网络!!",
			"PRINTER PROBLEEM, CONTROLEER NETWERK!!",
			"PRINTER PROBLEM, CHECK YOUR NETWORK!!",
			"PRINTER TIDAK BEKERJA, MEMERIKSA JARINGAN ANDA!!",
			"打印機不工作,,檢查您的網絡!!" },

	// _PRINT_QUANTITY

	{   "PRINT_QUANTITY", "打印多少?", "AANTAL AFDRUKKEN?", "PRINT HOW MANY TIMES?", "DRUCK ANZAHL",
		"PRINTER BERAPA KALI?", "打印多少?" },

	// _PROFIT

	{	"PROFIT", "利润", "WINST", "PROFIT", "GEWINN", "LABA", "利潤" },

	// _QUANTITY

	{	"QUANTITY", "数量", "HOEVEELHEID", "QUANTITY", "QUANTITAT", "KUANTITAS", "數量" },

	// _QUARTER

	{   "QUARTER", "季度年", "KWARTAAL", "QUARTER", "QUARTAL", "KWARTAL", "季度年" },

	// _QUICK_CHOOSE

	{   "QUICK_CHOOSE", "快速下单", "BESTEL SNEL", "CHOOSE QUICK", "SCHNELLBESTELLUNG", "PESANAN CEPAT", "快速下單" },

	// _RADICAL

	{   "RADICAL", "激进", "RADICAL", "RADICAL", "RADICAL", "RADICAL", "激進" },

	// _RECHAUD

	{   "RECHAUD", "炭烧盘", "RECHAUD", "RECHAUD", "RECHAUD", "RECHUAD", "炭燒盤" },

	// _REMARK

	{	"REMARK", "写备注", "OPMERKING", "REMARK", "NOTITZEN", "KETERANGAN", "寫備註" },

	// _REMOVE_ADD

	{ "REMOVE_ADD", "清除广告", "ADVERTENTIE WEGHALEN", "REMOVE ADVERTISEMENT",
	  "ENTFERNEN ADVERTISEMENT", "HAPUS IKLAN", "清除廣告" },

	// _REMOVE_CUSTOMER

	{ "REMOVE_CUSTOMER", "清除客户", "VERWIJDER KLANT", "KILL CLIENT", "KUNDE ENTFERNEN", "MENGHAPUS PELANGGANG", "清除客戶" },

	// _REMOVE_CUSTOMER_ASK

	{ "REMOVE_CUSTOMER_ASK", "清除客户?", "VERWIJDER KLANT?", "KILL CLIENT?", "KUNDE ENTFERNEN?", "MENGHAPUS PELANGGANG?", "清除客戶?" },

	// _REMOVE_MENU_CARD

	{ "REMOVE_MENU_CARD", "删除菜单", "MENUKAART VERWIJDEREN", "REMOVE MENU", "MENU LOSCHEN", "HAPUS MENU", "刪除菜單" },

	// _REMOVE_MONEY

	{ "REMOVE_MONEY", "取款", "GELD OPNEMEN", "REMOVE MONEY", "GELD ABHEBEN", "HAPUS UANG", "取款" },

	// _REMOVE_FOOD

	{ "REMOVE_FOOD", "清除食物", "ETEN WEG", "REMOVE FOOD", "ENTF.LEBENSMIT", "HAPUS HIDANGAN", "清除食物" },

	// _REMOVE_PART

	{ "REMOVE_PART", "清除食物", "ONDERDEEL VERWIJDEREN", "REMOVE PART", "ZUTATEN ENTFERNEN", "HAPUS BAHAN", "去除成分" },

	// _REMOVE_TAX_CLUSTER

	{ "REMOVE_TAX_CLUSTER", "清除税组", "CLUSTER WEGHALEN", "REMOVE CLUSTER", "KLAR STEUER", "JELAS CLUSTER", "清除稅組" },

	// _REPRINT

	{	"REPRINT", "重印", "HERDRUK", "REPRINT", "NEUDRUCK", "PRINT LAGI", "重印" },

	// _REROUTING

	{	"REROUTING", "  注意转印!!",
		" WAARSCHUWING HERROUTEN!!!",
		" WARNING!! REROUTING",
		", WARNING !!, REROUTING.",
	    ", PERINGATAN !!, PRINTER YANG LAIN.", " 注意轉印!!"
	},

	// _RESERVATION_GRAPH_USED_TABLE
	{ "RESERVATION_GRAPH_USED_TABLE", "晚餐餐桌", "EETTAFELS IN GEBRUIK", "DINNER TABLES IN USE",
	  "ESSTICH IM GEBRAUCH", "RUANG MAKAN DI GUNAKAN", "晚餐餐桌" },

	// _RESERVATION_GRAPH_FREE_TABLE
	{ "RESERVATION_GRAPH_FREE_TABLE", "餐桌不被使用", "TAFELS VRIJ", "DINING TABLES FREE",
			"ESSTISCHE NICHT BESETZT", "MEJA TIDAK TERJADI", "餐桌不被使用" },

	// _RESERVATION_GRAPH_USED_PERSON
	{ "RESERVATION_GRAPH_USED_PERSON", "目前的人", "PERSONEN AANWEZIG", "PERSONS PRESENT", "PERSONEN PRÄSENTIEREN", "ORANG HADIR", "目前的人" },

	// _RESERVATION_GRAPH_FREE_PERSON
	{ "RESERVATION_GRAPH_FREE_PERSON", "空座位", "VRIJE ZITPLAATSEN", "EMPTY SEATS", "LEERE PLÄTZE", "KURSI KOSONG", "空座位" },

	// _RESERVATION_GRAPH_USED_TABLES_PERSON
	{ "RESERVATION_GRAPH_USED_TABLES_PERSON", "目前使用的桌子人", "GEBRUIKTE TAFELS EN PERSONEN AANWEZIG", "USED TABLES AND PERSONS PRESENT",
			"VERWENDETE TISCHE UND PERSON ANWESEND", "PRESENT MEJA YANG DIGUNAKAN", "目前使用的桌子人" },

	// _RESERVATION_GRAPH_FREE_TABLES_PERSON
	{ "RESERVATION_GRAPH_FREE_TABLES_PERSON", "空晚餐桌和空座位", "VRIJE TAFELS EN VRIJE ZITPLAATSEN", "EMPTY DINNER TABLES AND EMPTY SEATS",
			"LEERE TISCHE UND LEERE SITZE", "MEJA MAKAN KOSONG DAN KURSI KOSONG", "空晚餐桌和空座位" },

	// _RESERVE_TABLE

	{ "RESERVE_TABLE", "订桌", "RESERVEER TAFEL", "RESERVE TABLE", "TISCH RESERVIEREN", "MEJA CADANGAN", "訂桌" },

	// _RESERVED

	{ "RESERVED", "RESERVED", "RESERVED", "RESERVED", "RESERVED", "RESERVED", "RESERVED" },

    // _RESET_PASSWORD

    { "RESET_PASSWORD", "重设密码", "HERSTEL PASWOORD", "RESET PASSWORD", "PASSWORT ZURÜCKSETZEN", "ULANG PASSWORD", "重設密碼" },

    // _RESTAMOUNT

	{	"RESTAMOUNT", "", "RESTBEDRAG", "AMOUNT LEFT", "BETRAG UBER",
		"JUMLAH KIRI", ""},

	// _RESTAURANT
	{	"RESTAURANT", "餐楼", "RESTAURANT", "RESTAURANT", "RESTAURANT", "RESTAURANT", "餐樓"	},

	// _RESTAURANT_PRICE
	{	"RESTAURANT_PRICE", "餐楼价格", "RESTAURANT PRIJS", "RESTAURANT PRICE", "RESTAURANT PREIS", "HARGA RESTAURANT", "餐樓價格"	},

	// _RESTAURANT_HALF_PRICE
	{	"RESTAURANT_HALF_PRICE", "餐楼半价格", "HALVE RESTAURANT PRIJS", "HALF RESTAURANT PRICE", "HALBE RESTAURANT PREIS",
		"HARGA SETENGAH RESTAURANT", "餐樓半價格"	},

	// _RETAILING33

	{	"RETAILING33", "[Graphics]销售统计", "VERKOOP STATISTIEK\n",
		"SALES STATISTICS\n", "KLEINVERK. STATISTICS\n", "PENJUALAN STATISTIK\n"},

	// _RETAILING_DELETED33

	{	"RETAILING33", "[Graphics]去除食品", "VERWIJDERD STATISTIEK\n",
		"REMOVED STATISTICS\n", "KLEINVERK. STATISTICS\n", "PENJUALAN STATISTIK\n",
	    "[Graphics]去除食品" },

    // _RETOUR_ITEMS

	{   "RETOUR_ITEMS", "退换货品", "RETOUR ARTIKELEN", "RETOUR ITEMS",
		"RÜCKGABE ARTIKEL", "MENGEMBALIKAN BARANG", "退換貨品" },

	// _RETAIL37

	{	"RETAIL37", "[Graphics]销售 : 服务员[Height:2]  ",
		"ORDER VERKOOP : SLEUTEL ",
		"ORDER SALE : KEY ", "ORDER VERKAUF : SCHLÜSSEL ",
		"SALE PEMESANAN: KUNCI",
		"[Graphics]銷售: 服務員[Height:2] "
	},

	// _RFID_KEY_TRANSPORT

	{	"RFID_KEY_TRANSPORT", "转单", "SLEUTEL TRANSPORT", "KEY TRANSPORT", "SCHLÜSSEL TRANSPORT",
	    "KUNCI TRANSPORTASI", "轉單"},

	// _ROW_TOP

	{ "ROW_TOP", "第一排", "EERSTE RIJ", "TOP ROW",
			"ERSTE REIHE", "BARIS PERTAMA", "第一排" },

	// _ROW_CENTER

	{ "ROW_CENTER", "中间排", "MIDDELSTE RIJ", "CENTER ROW",
			"MITTLEREN REIHE", "BARIS TENGAH", "中間排" },

	// _ROW_BOTTOM1

	{ "ROW_BOTTOM1", "最后一排1", "LAATSTE RIJ 1", "BOTTOM ROW 1",
			"LETSTE REIHE 1", "BARIS TERAKHIR 1", "最後一排1" },

	// _ROW_BOTTOM2

	{ "ROW_BOTTOM2", "最后一排2", "LAATSTE RIJ 2", "BOTTOM ROW 2",
			"LETSTE REIHE 2", "BARIS TERAKHIR 2", "最後一排2" },

	// _ROW_BUTTOM3

	{ "ROW_BOTTOM1", "最后一排3", "LAATSTE RIJ 3", "BOTTOM ROW 3",
			"LETSTE REIHE 3", "BARIS TERAKHIR 3", "最後一排3" },

	// _SALES_GRAPH

	{   "SALES_GRAPH", "统计", "STATISTIEKEN", "STATISTICS", "STATISTIK",
			"STATISTIKA", "統計" },

	// _SALES_GRAPH_INFO

	{   "SALES_GRAPH_INFO", "销售信息", "VERKOOP INFORMATIE", "SALES INFORMATION",
		"VERTRIEBS INFORMATIONEN", "INFORMASI PENJUALAN", "銷售信息" },

	// _SALES_GRAPH_TYPE

	{   "SALES_GRAPH_TYPE", "图表", "TYPE GRAFIEK", "DIAGRAM", "DIAGRAMM", "DIAGRAM", "圖表" },

	// _SAVE

	{	"SAVE", "入机", "SAVE", "SAVE", "SAVE", "SAVE", "入機" },

	// _SEARCH

	{ "SEARCH", "检索", "ZOEKEN", "SEARCH", "SUCHEN", "CARI", "檢索" },

	// _SEARCH_CUSTOMER

	{ "SEARCH_CUSTOMER", "检索", "ZOEKEN", "SEARCH", "SUCHEN", "CARI", "檢索" },

	// _SELECT_ALL

	{ "SELECT_ALL", "一切", "ALLES DIRECT KIEZEN", "ALL NOW", "ALLE WAHLEN", "SEMUANYA", "一切" },

	// _SEND_BILL

	{ "SEND_BILL", "银行", "OP REKENING", "ACCOUNT", "ACCOUNT", "POS", "銀行" },

	// _SEND_NEW_TIME

	{ "SEND_NEW_TIME", "新的交货时间", "STUUR NIEUWE LEVERTIJD", "SEND NEW TIME", "NEUE LIEFERZEIT", "WAKTU PENGIRIMAN BARU", "新的交貨時間" },

	// _SEQ_TABLE_KITCHEN_TOTAL

	{	"SEQ",
		"[Ifchars:35]号码 [Endif]台子    [subtab:30]      酒吧    厨房    总",
		"[Ifchars:35]SEQ [Endif]TAFEL    [subtab:30]      BAR    KEUKEN    TOTAAL",
		"[Ifchars:35]NR. [Endif]TABLE    [subtab:30]      BAR   KITCHEN     TOTAL",
		"[Ifchars:35]NR. [Endif]TISCH    [subtab:30]      BAR     K}CHE     TOTAL",
		"[Ifchars:35]NO [Endif]DAPUR     [subtab:30]      BAR     TABEL    JUMLAH",
		"[Ifchars:35]號碼[Endif]台子[subtab:30] 酒吧廚房總" },

	// _SEQA

	{	"SEQA",
		"号码 日 台子[subtab:20]      现金 PIN/KAART",
		"NR. DATUM NUMMER[subtab:20]      CASH PIN/KAART",
		"SEQ DATE  NUMBER[subtab:17]   CASH      CARD",
		"SEQ DATUM NUMMER[subtab:16]  CASH     KARTE",
		"SEQ NOMOR TANGGAL[subtab:17] KARTU    KAS",
		"號碼日台子[subtab:20] 現金PIN/KAART" },

	// _SEQ1

	{	"SEQ1",
		"[Graphics]号码  时间  台子[Subtab:32] 总",
		"NR. TIJD  TAFEL[subtab:6]TOTAAL",
		"SEQ TIME  TABLE[subtab:5]TOTAL",
		"SEQ ZEIT  TISCH[subtab:5]TOTAL",
		"SEQ WAKTU TABLE[subtab:5]TOTAL",
		"[Graphics]號碼  時間  台子[Subtab:32] 總" },

	// _SEQ1A

	{	"SEQ1A", "[Graphics]号码  时间  台子[subtab:112]存款[Subtab:32] 总",
		"NR. TIJD  NUMMER[subtab:20]AANBETAALD    TOTAAL",
		"SEQ TIME  NUMBER[subtab:17]DEPOSIT     TOTAL",
		"SEQ ZEIT  NUMMER[subtab:16]ANBEZ.     TOTAL",
		"SEQ WAKTU NOMOR[subtab:16]TOTAL DEPOSIT",
		"[Graphics]號碼  時間  台子[subtab:112] 存款[Subtab:32] 總" },

	// _SEQ2

	{	"SEQ2",
		"[Graphics]号码  日  台子 [Subtab:32] 总",
		"NR. DATUM TAFEL[subtab:6]TOTAAL",
		"SEQ DATE  TABLE[subtab:5]TOTAL",
		"SEQ DATUM TISCH[subtab:5]TOTAL",
		"SEQ TANGGAL TABLE[subtab:5]TOTAL",
		"[Graphics]號碼日台子[Subtab:32] 總"
	},

	// _SEQ47

	{	"SEQ47", "[Graphics|Ifndots:176|DPI:1|End]号码  台子 [Subtab:32]数量\n",
		"[Text|Ifnchars:33|Font:0|End|Ifchars:32]SEQ [End]TAFEL NAAM[SUBTAB:7]HOEVEEL\n",
		"[Text|Ifnchars:33|Font:0|End|Ifchars:32]SEQ [End]TABLE NAME[SUBTAB:8]QUANTITY\n",
		"[Text|Ifnchars:33|Font:0|End|Ifchars:32]SEQ [End]TISCH NAME[SUBTAB:9]QUANTITAT\n",
		"[Text|Ifnchars:33|Font:0|End|Ifchars:32]SEQ [End]NAMA TABLE[subtab:8]JUMLAH\n",
		"[Graphics|Ifndots:176|DPI:1|End]號碼  台子[Subtab:32]數量\n"
	},

	// _SERVICE_KEY

	{ "SERVICE_KEY", "服务密钥", "SERVICE SLEUTEL", "SERVICE KEY", "DIENST SCHLUSSEL", "KUNCI LANAYAN", "服務密鑰" },

	// _SHIFT

	{  "SHIFT", "SHIFT", "SHIFT", "SHIFT", "SHIFT", "SHIFT", "SHIFT" },

	// _SHOP

	{  "SHOP", "商店", "WINKEL", "SHOP", "GESCHÄFT", "TOKO", "商店" },

	// _SHOW_NUMBERS

	{  "SHOW_NUMBERS", "显示数量", "NUMMERS", "NUMBERS", "NUMMERS", "JUMLAH", "顯示數量" },

	// _SHOW_PERSONS

	{  "SHOW_PERSONS", "人", "PERSONEN", "PERSONS", "PERSONEN", "ORANG", "人" },

	// _SHUTDOWN
	{
		"SHUTDOWN", "功率下降", "UITZETTEN", "SHUT DOWN", "KASSA AUS", "SHUT DOWN", "功率下降"
	},

	// _SIMPLIFIED
	{
		"SIMPLIFIED", "简体", "SIMPLIFIED", "SIMPLIFIED", "SIMPLIFIED", "SIMPLIFIED", "簡體"
	},

	// _SPLIT_TABLE
	{
		"SPLIT_TABLE", "菜单移动", "TAFEL OPSPLITSEN",
		"TABLE DIVIDE", "TISCH SPALTUNG",
		"MEJA PEMISAHAN", "菜單移動"
	},

	// _START_COOKING
	{
		"START_COOKING", "开始做饭", "START BEREIDEN", "START COOKING", "FANG AN ZU KOCHEN", "MULAI MEMASAK", "開始做飯"
	},

	// _STATE

	{	"STATE", "状况", "STATUS", "STATE", "STATUS", "STATUS", "狀況"	},

    // _STATIEGELD

    {   "STATIEGELD", "存款", "STATIEGELD", "DEPOSIT", "PFAND", "STATIEGELD", "訂金"  },

	// _STORE_EMPTY_DATE,

	{
		"STORE_EMPTY_DATE", "日期结束", "DATUM EINDE VOORRAAD", "DATE END OF STOCK", "DATUM ENDE DER LAGER", "TANGGAL AKHIR STOK", "日期結束"
	},

	// _STORE_STATUS_CANCELLED

	{	"STORE_STATUS_CANCELLED", "停止交货", "ANNULEER LEVERING", "CANCEL DELIVERY", "STOPPEN LIEFERUNG", "BATALKAN PESANAN", "停止交貨" },

	// _STORE_STATUS_CONSUMED

	{  "STORE_STATUS_CONSUMED", "总消费量", "TOTAAL GEBRUIKT",
			"TOTAL USED", "GESAMT VERBRAUCHT", "TOTAL DIKONSUMSI", "總消費量" },

	// _STORE_STATUS_DELIVERED

	{
		"DELIVERED", "订单已交付", "GELEVERD", "DELIVERED", "GELIEFERT", "DISEDIAKAN", "訂單已交付"
	},

	// _STORE_STATUS_DELIVERED_EARLY

	{
		"STORE_STATUS_DELIVERED_EARLY", "收到的", "VROEG GELEVERD", "EARLY DELIVERY", "FRUH GELIEFERT", "DITERIMA SEBELUMNYA", "收到的"
	},

	// _STORE_STATUS_DELIVERED_TOO_LATE

	{ "STORE_STATUS_DELIVERED_TOO_LATE", "交付太晚了", "TE LAAT GELEVERD", "DELIVERED TOO LATE", "LIEFERUNG STORNIERT", "PENGIRIMAN DIBATALKAN", "交付太晚了" },

	// _STORE_STATUS_EVER_DELIVERED

	{
		"STORE_STATUS_EVER_ORDERED", "总共交付", "TOTAAL OOIT GELEVERD", "TOTAL EVER DELIVERED", "TOTAL ALLES GELIEFERT", "TOTAL PERNAH DIBERIKAN", "總共交付"
	},

	// _STORE_STATUS_NOT_ORDERED_YET

	{
		"STORE_STATUS_NOT_ORDERED_YET", "尚未订购", "NOG NIET BESTELD", "NOT ORDERED YET", "NOCH NICHT BESTELLT", "BELUM DIPESAN", "尚未訂購"
	},

	// _STORE_STATUS_ORDERED

	{	"STORE_STATUS_ORDERED", "有序", "BESTELD", "ORDERED", "BESTELLT", "DIPESAN", "有序"	},

	// _STORE_STATUS_PREPARED

	{	"STORE_STATUS_PREPARED", "准备", "VOORBEREID", "PREPARED", "BEREIT", "SIAP", "準備"	},

	// _STORE_TOTAL_ITEMS

	{  "STORE_TOTAL_ITEMS", "总库存", "TOTAAL VOORRAAD", "TOTAL STOCK", "GESAMTZAHL", "JUMLAH TOTAL", "總庫存" },

	// _STORE_STATUS_YEAR_CONSUMPTION

	{   "STORE_STATUS_YEAR_CONSUMPTION", "年使用", "JAARVERBRUIK", "ONE YEAR USE", "JAHR GEBRAUCH", "PENGGUNAAN TAHUN", "年使用" },

	// _STYLE

	{   "STYLE", "风格", "STIJL", "STYLE", "STIL", "STYLE", "風格" },

	// _SUBTOTAL

	{	"SUBTOTAL", "合计", "SUBTOTAAL", "SUBTOTAL", "SUBTOTAL", "KHUSUS", "合計"},

	// _SUPPLIER

	{	"SUPPLIER", "供应商", "LEVERANCIER", "SUPPLIER", "LIEFERANT", "PEMASOK", "供應商"},

	// _SUPPLIER_ADD

	{	"SUPPLIER", "添加供应商", "LEVERANCIER NIEUW", "ADD SUPPLIER", "LIEFERANT NEU", "PEMASOK", "添加供應商"},

	// _SUPPLIER_REMOVE

	{	"SUPPLIER", "删除供应商", "LEVERANCIER WEG", "REMOVE SUPPLIER", "LIEFERANT FORT", "HAPUS PEMASOK", "刪除供應商"},

	// _SUSHI

	{ "SUSHI", "寿司", "SUSHI", "SUSHI", "SUSHI", "SUSHI", "壽司" },

	// _SWITCH_OFF

	{ "SWITCH_OFF", "你想关掉电脑呢？", "KASSA UITZETTEN?", "DO YOU WANT TO SWITCH OFF THE PC?",
	  "WOLLEN SIE ZUM ABSCHALTEN DES PC?", "APAKAH ANDA INGIN MEMATIKAN PC?",
	  "你想關掉電腦呢？"
	},

	// _UNDO_CHANGES

	{	"UNDO_CHANGES", "取消更改", "VERANDERINGEN TERUGDRAAIEN", "UNDO CHANGES", "VERÄNDERUNGEN RÜCKGÄNGIG MACHEN", "TIDAK JADI DI RUBAH", "取消更改" },

	// _USB
	{ "USB", "USB闪存盘", "USB DISK", "USB STICK", "USB STICK", "USB STICK", "USB閃存盤" },

	// _TABLE

	{	"TABLE", "餐桌", "TAFEL", "TABLE", "TISCH", "MEJA", "餐樓" }, // _TABLE

	// _TABLET_UPDATE

	{   "TABLET_UPDATE", "片剂下载", "TABLET UPDATE", "TABLET UPDATE", "TABLET UPDATE", "TABLET UPDATE", "片劑下載" },

	// _TAKEAWAY_PHONE

	{ "TAKEAWAY_PHONE",  "电话订餐", "TELEFONISCH BESTELLEN MET KEUZEPAGINA", "ORDER BY PHONE WITH PAGES",
	  "ORDER TELEFON MIT PAGINA", "ORDER MOBIL DENGAN PESANAN", "電話訂餐"},

	// _TAKEAWAY_PRICE,
	{ "TAKEAWAY_PRICE", "打包价格", "PRIJS AFHALEN", "PRICE TAKEAWAY", "PREIS MITNEHMEN", "HARGA KELUAR", "打包價格" },

	// _TAKEAWAY_HALF_PRICE,
	{ "TAKEAWAY_HALF_PRICE", "打包半价格", "PRIJS AFHALEN HALVE PORTIE", "TAKEAWAY HALF PORTION",
      "MITNEHMEN HALBE PORTION", "HARGA KELUAR SETENGAH", "打包半價格" },

	// _TAX4

	{	"TAX4", "税额", "BTW", "TAX", "MwSt", "PPN", "稅額" },

	// _TAX_GROUP

	{	"TAX_GROUP", "税额", "BTW GROEP", "TAX GROUP", "MwSt. GRUPPE", "TAX GROUP", "稅額" },

	// _TELEPHONE

	{	"TELEPHONE", "电话", "TEL.", "TEL.", "TEL.", "TEL.", "電話" },

	// _TELEPHONE

	{	"TELEPHONE", "电话", "TELEFOON", "TELEPHONE", "TELEFON", "TELEPON", "電話" },

	// _TEMPORARY_PRICE

	{	"TEMPORARY_PRICE", "暂时价格", "TIJDELIJKE PRIJZEN", "TEMPORARY PRICES", "VORLAUFIGE PREISE",
		"HARGA SEMENTARA", "暫時價格" },

	// _TEXT_COLOUR

	{   "TEXT_COLOUR", "文字颜色", "TEKST KLEUR", "TEXT COLOUR", "TEKST FARBE", "WARNA TEKS", "文字顏色" },

	// _THUIS

	{  "THUIS", "THUIS", "THUIS", "THUIS", "THUIS", "THUIS", "THUIS" },

	// _THUISBEZORGD

	{  "THUISBEZORGD", "THUISBEZORGD", "THUISBEZORGD", "THUISBEZORGD", "THUISBEZORGD", "THUISBEZORGD", "THUISBEZORGD" },

	// _TICKET

	{	"TICKET", "收据", "BON", "TICKET", "KASSENBON", "RESI", "收據" },

	// _TIME2

	{	"TIME2", "时间", "TIJD", "TIME", "ZEIT", "WAKTU", "時間" },

	// _TIPS

	{	"TIPS", "小账", "FOOI", "TIPS", "BONUS", "TIP", "小賬" },

	// _TODAY

	{	"TODAY", "今天", "VANDAAG", "TODAY", "HEUTE", "HARI INI", "今天" },

	// _TOTAL

	{	"TOTAL", "总数", "TOTAAL", "TOTAL", "TOTAL", "JUMLAH", "總數" },

	// _TOTAL_DRAWER_CHANGES

	{   "TOTAL_DRAWER_CHANGES", "取钱器更换", "GELDLA WIJZIGINGEN", "MONEY CHANGE", "GELD ANDERUNGEN", "UANG LACI BERUBAH", "取錢器更換" },

	// _TOTAL_WITHOUT_DISCOUNT

	{   "TOTAL_WITHOUT_DISCOUNT", "", "TOTAAL ZONDER KORTING", "TOTAL MINUS DISCOUNT", "GESAMT OHNE RABATT", "TOTAL TANPA DISKON", "" },

	// _TOTALOFDAY

	{	"TOTALOFDAY", "[Text|Font:1]%d[Subtab:8]%s\n",
		"[Text|Font:1]%d[Subtab:8]%s\n",
		"[Text|Font:1]%d[subtab:8]%s\n",
		"[Text|Font:1]%d[Subtab:8]%s\n",
		"[Text|Font:1]%d[Subtab:8]%s\n",
		"[Text|Font:1]%d[Subtab:8]%s\n" },

	// _TRADITIONAL

		{ "TRADITIONAL", "正体字", "ORIGINELE LETTERS", "TRADITIONAL CHARACTERS",
		  "TRADITIONAL", "TRADITIONAL", "正體字" },

	// _TRANSACTION

	{ "TRANSACTION", "买卖", "TRANSACTIE", "TRANSACTION", "TRANSACTIE", "TRANSAKSI", "買賣" },

	// _TRANSACTIONS

	{ "TRANSACTIONS", "买卖", "TRANSACTIES", "TRANSACTIONS", "TRANSACTIES", "TRANSAKSI", "買賣" },

	// _TRANSPORT

	{	"TRANSPORT", "转结燎", "TRANSPORT", "TRANSPORT", "TRANSPORT", "TRANSPORTASI", "轉結燎"},

	// _TWIN_ITEM
	{   "TWIN_ITEM", "餐双", "TWIN", "TWIN ITEM", "TWIN ARTIKEL", "KEMBAR MAKANAN", "餐雙" },

	// _TYPE_ITEM_ACCESSOIRE
	{   "TYPE_ITEM_ADDITION", "配件", "ACCESSOIRE",
	    "SALES ITEM ACCESSOIRE", "ZUBEHÖR", "AKSESORI", "配飾" },

	// _TYPE_MENU_ADD
	{   "TYPE_MENU_ADD", "新菜单", "MENU NIEUW", "NEW MENU ITEMS", "NEUE MENU ARTIKELEN", "MENU BARU", "新菜單" },

	// _TYPE_MENU_COMBINATION
	{   "TYPE_MENU_COMBINATION", "特别餐", "SPECIAAL ETEN", "SPECIAL DISH", "SPEZIEL", "MAKANAN COMBINASI	", "特別餐" },

	// _TYPE_MENU_DESCRIPTION
	{   "TYPE_MENU_DESCRIPTION", "香料", "SPECERIJEN", "SPICES", "GEWURZE", "BUMBU BUMBU", "香料" },

	// _TYPE_MENU_EXTRA
	{   "TYPE_MENU_EXTRA", "小菜", "BIJGERECHT", "SIDE DISH", "GEWURZE", "BUMBU BUMBU", "小菜" },

	// _TYPE_MENU_ITEM
	{   "TYPE_MENU_ITEM", "菜单", "MENU ITEM", "MENU ITEM", "MENU ITEM", "MENU ITEM", "菜單項" },

	// _TYPE_SALES_ITEM
	{   "TYPE_MENU_ARTICLE", "单项", "VERKOOP ARTIKEL", "SALES ITEM", "VERKAUFSARTIKEL", "BARANG PENJUALAN", "單項" },

	// _TYPE_OUTOFSTOCK
	{   "TYPE_OUTOFSTOCK", "脱销", "GEEN VOORRAAD", "NO STOCK", "KAN NICHT MACHEN", "TIADA STOCK", "脱销" },

	// _TYPE_SEPARATOR
	{   "TYPE_SEPARATOR", "分离器", "HORIZONTALE LIJN", "HORIZONTAL LINE IN BILL", "HORIZONTALE LINIE", "SEPARATOR", "分離器" },

	// _UPDATE_ITEM_EXISTING
	{   "UPDATE_ITEM_EXISTING", "更改所选菜单项", "BESTAAND MENU ARTIKEL AANPASSEN",
			"CHANGE CURRENT MENU ITEM", "ÄNDERN SIE DEN AUSGEWäHLTEN MENU",
			"UBAH MENU YANG DIPILIH", "更改所選菜單項" },

	// _UPDATE_OR_NEW
	{   "UPDATE_OR_NEW", "新菜单文章或更新？", "NEW MENU ARTICLE OR AN UPDATE?",
		"NIEW ARTIKEL OF EEN UPDATE?", "NEUER MENÜ ODER EIN UPDATE?",
		"MENU BARU ATAU UPDATE?", "新菜單文章或更新？" },

	// _UPDATE_SOFTWARE

	{   "UPDATE_SOFTWARE", "更新软件", "SOFTWARE UPDATE", "SOFTWARE UPDATE", "SOFTWARE UPDATE", "MEMPERBARUI SOFTWARE", "更新軟件" },

	// _UPDATE_TO_NEW_ITEM,
	{   "UPDATE_TO_NEW_ITEM", "更改为新菜单项", "EEN NIEUW MENU ARTIKEL AANMAKEN",
		"UPDATE TO A NEW MENU ITEM", "WECHSELN ZU EINEM NEUEN MENU", "UBAH KE MENU BARU", "更改為新菜單項" },

	// _UPLOAD_ARCHIVE

	{   "UPLOAD_ARCHIVE", "上传存档", "UPLOAD ARCHIEF","UPLOAD ARCHIVE","ARCHIV HOCHLADEN","UPLOAD ARCHIVE","上傳存檔" },

	// _UPLOAD_CONFIGURATION

	{   "UPLOAD_CONFIGURATION", "UPLOAD CONFIGURATION", "UPLOAD CONFIGURATION", "UPLOAD CONFIGURATION", "UPLOAD CONFIGURATION", "UPLOAD CONFIGURATION", "UPLOAD CONFIGURATION" },

	// _UPLOAD_LOG

	{   "UPLOAD_LOG", "CONNECT", "CONNECT", "CONNECT", "CONNECT", "CONNECT", "CONNECT" },

	// _UPLOAD_MENU

	{   "UPLOAD_MENU", "上传菜单", "MENU BEWAREN", "SAVE MENU", "MENU HOCHLADEN", "MENU UNGGAH", "上傳菜單"	},

	// _USE_FOR

	{ 	"USE_FOR", "使用给食物", "GEBRUIK BIJ..", "USE AT FOOD..", "VERWENDEN MIT", "DENGAN MAKANAN..", "使用給食物" },

	// _USE_FOR_RESTAURANT

	{ 	"_USE_FOR_RESTAURANT", "使用给餐楼", "GEBRUIK BIJ RESTAURANT",
		"USE AT RESTAURANT", "VERWENDEN MIT RESTAURANT", "DENGAN RESTAURANT", "使用給餐樓" },

	// _USE_FOR_TAKEAWAY

	{ 	"_USE_FOR_TAKEAWAY", "使用给外卖", "GEBRUIK BIJ AFHALEN", "USE AT TAKEAWAY",
		"VERWENDEN ABHOLEN", "DENGAN COLLECTION", "使用給外賣" },

	// _USE_FOR_TELEPHONE

	{ 	"_USE_FOR_TELEPHONE", "使用给送外卖", "GEBRUIK BIJ BEZORGEN",
		"USE AT FOOD..", "VERWENDEN MIT", "DENGAN MAKANAN..", "使用給食物" },

	// _VERTALEN
	{   "VERTALEN", "食品", "VERTAAL", "TRANSLATE", "ESSEN", "MAKANAN", "食品" },

	// _VIKI

	{	"VIKI", "VIKI 网络电视", "VIKI TV", "VIKI TV", "VIKI TV", "VIKI 網路電視" },

	// _VISA

	{	"VISA", "VISA", "VISA", "VISA", "VISA", "VISA" },

	// _WAIT

	{   "WAIT", "请等一会儿", "EVEN WACHTEN", "WAIT A MOMENT", "MAL WARTEN", "SEBENTAR YA", "請等一會兒" },

	// _WAIT_DATABASE

	{   "WAIT_DATABASE", "请等一会儿", "ONTVANG GEGEVENS VAN DATABASE...", "WAIT FOR DATABASE..T",
			"MAL WARTEN", "SEBENTAR YA UNTUK DATABASE", "請等一會兒" },

	// _WALL

	{   "WALL", "墙壁", "MUREN", "WALL", "WAND", "DINDING", "牆壁" },

	// _WALL_CHOOSE

	{   "WALL_CHOOSE", "选择墙", "MUUR KIEZEN", "CHOOSE WALL", "WAND WAHLEN", "MEMILIH DINDING", "選擇牆" },

	// _WALL_COLOUR

	{   "WALL_COLOUR", "墙面色彩", "MUUR KLEUR", "WALL COLOUR", "WAND FARBE", "WARNA DINDING", "牆面色彩" },

	// _WANTED_TIME

	{   "WANTED_TIME", "顾客时间", "GEWENSTE TIJD", "CUSTOMER TIME", "KUNDENZEIT", "WAKTU PELANGGAN", "顧客時間" },

	// _WARNING_NO_CHANGES
	{
        "WARNING_NO_CHANGES", "未发现任何更改", "GEEN WIJZIGINGEN!!",
		"WARNING, NO CHANGES!!",
		"KEINE ANDERUNGEN", "TIDAK ADA PERUBAHAN YANG DITEMUKAN", "未發現任何更改"
	},

	// _WEEK_DAYS

	{
	    "WEEK_DAYS", "星期几", "WEEKDAGEN", "WEEK DAYS", "TAG WOCHE", "HARI APA", "星期幾"
	},

	// _WIDTH

	{   "WIDTH", "宽度", "BREEDTE", "WIDTH", "BREITE", "LEBAR", "寬度" },

	// _WOK

	{   "WOK", "炒锅", "WOK", "WOK", "WOK", "WAJAN", "炒鍋" },

	// _YEAR

	{	"YEAR", "年", "JAAR", "YEAR", "JAHR", "TAHUN", "年" },

	// _CHEF_FUNCTION

	{	"CHEF_FUNCTION", "老板功能", "MANAGER FUNCTIES", "MANAGER FUNCTIONS", "CHEF FUNCTION", "FUNGSI MANAJER", "老闆功能" },

	// _KEY_FUNCTION

	{	"KEY_FUNCTION", "查看锁匙数", "SLEUTEL KIJKEN",
		"KEY CHECKING", "SCHLÜSSEL BERICHT", "KUNCI MEMERIKSA", "查看鎖匙數"},

	// _KEY_BAD

	{	"KEY_REPLACE", "请用别条匙", "ANDERE SLEUTEL GEBRUIKEN!",
		" PLEASE USE OTHER KEY!", "NICHT MEHR DIESE SCHLÜSSEL!",
		"MOHON GUNAKAN, KUNCI LAINNYA", "請用別條匙" },

	// _PAGE_ORDER

	{	"PAGE_ORDER", "餐类入单", "BESTELLEN MET KEUZEPAGINA", "ORDER WITH PAGES", "ORDER MIT PAGINA", "ORDER DENGAN PESANAN", "餐類入單"},

	// _BILL

	{	"BILL", "买单", "NOTA", "BILL", "KASSE", "NOTA", "買單" },

	// _BILL_OPTION

	{	"BILL_OPTION", "哪里买单", "KIES AFREKENEN", "CHOOSE BILL", "ZUR KASSE", "PILIH NOTA", "哪裡買單" },

	// _TELEPHONE_LOG

	{	"TELEPHONE_LOG", "送餐时间", "BEZORG TIJDEN", "DELIVERY TIMES",
			 "BESORG ZEITEN", "CONCERN TIMES", "送餐時間" },

	// _TELEPHONE_ORDER

	{	"TELEPHONE_ORDER", "", "", "", "", "" },

	// _KEY

	{	"KEY", "匙", "SLEUTEL", "KEY", "SCHLÜSSEL", "KUNCI", "匙" },

	// _KEY_ERASING

	{	"KEY_ERASING", "结数後洗匙", "SLEUTEL ARCHIVEREN",
		"ARCHIVE KEY", "SCHLÜSSEL ARCHIVIEREN", "KUNCI DELETE", "結數後洗匙" },

	// _KEY_ERASING_FIRST

	{	"KEY_ERASING_FIRST", "之前结数後洗匙!", "EERST SLEUTEL ARCHIVEREN GRAAG!",
		"ARCHIVE KEYS FIRST!", "ZUERST SCHLÜSSEL ARCHIVIEREN!", "PERTAMA KUNCI DELETE!", "之前結數後洗匙!" },

	// _REPORT

	{	"REPORT", "会计报告", "RAPPORT", "ACCOUNT REPORT", "RAPPORT", "MANAGER DAFTAR", "會計報告", },

	// _TABLE_MAP_ORDER

	{   "TABLE_MAP_ORDER", "平面图", "PLATTEGROND", "FLOORPLAN", "GRUNDRISS", "PLATTEGROND RESTORAN", "平面圖" },

	// _TABLE_MAP_DESIGN

	{   "TABLE_MAP_DESIGN", "设计平面图", "ONTWERP PLATTEGROND", "", "DESIGN-GRUNDRISS", "DESAIN RENCANA", "設計平面圖" },

	// _TABLE_MAP_TABLES

	{   "TABLE_MAP_TABLES", "移动表", "TAFELS HERSCHIKKEN", "MOVE AND PLACE TABLES", "TISCH ORDER", "MEJA", "移動表" },

	// _TIME_CORRECTION

	{	"TIME_CORRECTION", "平面图", "TIJD CORRECTIE", "TIME CORRECTION", "ZEIT KORRECTUR", "JAM, KOREKSI", "平面圖" },

	// _TIME_ERROR

	{	"TIME_ERROR", "不合时机", "TIJD FOUT", "TIME WRONG", "ZEIT FALSCH", "JAM TIDAK BISA", "不合時機" },

	// _KEY_SUMMATION

	{	"KEY_SUMMATION", "锁匙结数", "SLEUTEL SOM", "KEY SUMMATION", "SCHLÜSSEL SUMMER", "KUNCI, SUM", "鎖匙結數" },

	// _KEY_CONTROL

	{	"KEY_CONTROL", "查锁匙数", "BOEKHOUDING SLEUTEL", "KEY ACCOUNTING",
		"SCHLÜSSEL ABRECHNEN", "AKUNTANSI KUNCI", "查鎖匙數" },

	// _REMOVE_KEY

	{	"REMOVE_KEY", "请删除钥匙圈", ", VERWIJDER SLEUTEL",
		", REMOVE KEY", ", SCHLÜSSEL ENTFERNEN", "HAPUS ORDER", "請刪除鑰匙圈" },

	// _POSTCODE   box 4x2

	{	"POSTCODE", "邮政编码", "POST CODE", "POST CODE", "POST CODE", "POSTCODE", "郵政編碼"},

	// _DELIVER_TIME,

	{	"DELIVER_TIME", "出餐", "LEVERTIJD", "TIME NOTE", "LIEFERZEIT", "JAM", "出餐" },

	// _HOUSENR

	{	"HOUSENR", "门牌", "HUISNR.", "NUMBER", "NUMMER", "NOMOR", "門牌" },

	// _STREET
	{   "STREET", "街道", "STRAAT", "STREET", "STRASSE", "JALAN", "街道" },

    // _VERTICAL_SCROLL
    {
		  "VERTICAL_SCROLL", "水平滑动", "SCROLL\nVERTIKAAL", "VERTIKAL\nSCROLL",
		  "WISCHEN\nVERTIKAL", "GESER\nVERTIKAL", "垂直滑動"
	},

	// _VISIBLE_ITEM
	{     "VISIBLE_ITEM", "可见的", "ZICHTBAAR", "VISIBLE ITEM", "SICHTBAR", "TERLIHAT", "可見的"
 	},

	// _WALLET
	{ "WALLET", "钱包", "CONTANT GELD VOORRAAD", "WALLET", "BRIEFTASCHE", "DOMPET", "錢包" },

	// _WARN_KEY

	{	"WARN_KEY", "请删除坏钥匙圈", "WAARSCHUWING SLEUTEL SLECHT",
		"KEYRING DEFECT!",
		" SCHLÜSSEL WARNUNG", "LAINNYA KEYCHAIN!", "請刪除壞鑰匙圈" },

	// _WIDTH_TABLE

	{   "WIDTH_TABLE", "宽度", "TAFEL BREEDTE", "TABLE WIDTH", "TISCH BREITE", "MEJA LEBAR",  "寬度" },
};

EtextId findTextId( const std::string &textId)
{
	int i;
	for (i = 0; i < _NO_TEXT; i++)
	{
		if ( Lin[i][0].compare( textId)==0)
		{
			return (EtextId)i;
		}
	}
	return _NO_TEXT;
}

std::string getDate(int month, int day, int year)
{
    try
    {
        char t[256];

        if (PrintLang == LANG_SIMPLIFIED || PrintLang ==LANG_TRADITIONAL)
            sprintf(t, "%d-%d-%d", day, month, year);
        else
            sprintf(t, "[Width:1|Height:1]%d %s %d", day,
                    getTranslation( _JANUAR + month - 1, PrintLang).c_str(), year);
        return t;
    }
    catch(...)
    {}
    return std::string();
}

/** @brief Get a single translation
 *  @param id [in] Which text string
 *  @param language [in] Which language
 *  @return String defined
 */
std::string getTranslation( int id, Taal language)
{
	if ( language ==LANG_CURRENT)
	{
		language =getLanguage();
	}
	if ( id>=0)
	{
		return Lin[(EtextId)id][language];
	}
	return "";
}
