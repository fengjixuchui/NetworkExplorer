#pragma once

#include <functional>
#include "ViewBase.h"
#include "VirtualListView.h"
#include "NetworkInformation.h"

class CNetworkInfoView :
	public CViewBase<CNetworkInfoView>,
	public CVirtualListView<CNetworkInfoView>,
	public CCustomDraw<CNetworkInfoView> {
public:
	DECLARE_WND_CLASS(nullptr)

	CNetworkInfoView(IMainFrame* frame) : CViewBase(frame) {}

	CString GetColumnText(HWND, int row, int col);

	PCWSTR GetHeader() const;

	virtual void OnFinalMessage(HWND /*hWnd*/);

	static PCWSTR NodeTypeToString(UINT type);
	static PCWSTR InterfaceTypeToString(IFTYPE type);
	static CString AddressTypeToString(WORD type);

	BEGIN_MSG_MAP(CNetworkInfoView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnTreeItemChanged)
		CHAIN_MSG_MAP(CVirtualListView<CNetworkInfoView>)
		CHAIN_MSG_MAP(CCustomDraw<CNetworkInfoView>)
		CHAIN_MSG_MAP(CViewBase<CNetworkInfoView>)
	ALT_MSG_MAP(1)
		CHAIN_MSG_MAP_ALT(CViewBase<CNetworkInfoView>, 1)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

private:
	enum GenericItemType {
		// general
		HostName, DomainName, DnsServerList,
		NodeType, ScopeId, EnableRouting, EnableProxy, EnableDns,

		// adapter
		AdapterIfIndex, AdapterName, AdapterDnsSuffix, AdapterDesc, AdapterFriendlyName,
		AdapterPhyAddr, AdapterIfType,

		// interface
		IfAlias, IfDesc, IfGuid, IfLuid, IfIndex, IfType,
	};
	struct GenericItem {
		GenericItemType Type;
		CString Property;
		std::function<CString()> ValueFn;
		CString Value;

		GenericItem(GenericItemType type, PCWSTR name, std::function<CString()> fn) : Type(type), Property(name), ValueFn(fn) {}
		GenericItem(GenericItemType type, PCWSTR name, const CString& value) : Type(type), Property(name), Value(value) {}
	};

	void SetAdapterNodeItems(const AdapterInfo& adapter);
	void SetInterfaceNodeItems(const InterfaceInfo& iface);
	void SetGeneralNodeItems();
	void SetListViewPropertyColumns();
	void BuildTree();
	void UpdateList();
	void AddAdapters(CTreeItem parent);
	void AddInterfaces(CTreeItem parent);
	void SetIPTableNodeItems();

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTreeItemChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

	CSplitterWindow m_Splitter;
	CTreeViewCtrlEx m_Tree;
	CListViewCtrl m_List;
	CTreeItem m_SelectedNode, m_GeneralNode, m_AdaptersNode, m_InterfacesNode, m_IPTableNode;
	BYTE m_FixedInfoBuffer[1 << 10];
	PFIXED_INFO m_FixedInfo;
	std::vector<InterfaceInfo> m_Interfaces;
	std::vector<AdapterInfo> m_Adapters;
	std::vector<GenericItem> m_Items;
	std::vector<MIB_IPADDRROW> m_IPTable;
	bool m_IsGenericNode{ false };
};

