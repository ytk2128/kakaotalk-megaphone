#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <vector>
#include <thread>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unordered_map>

#pragma comment(lib, "user32.lib")

namespace Megaphone {
	using namespace System;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace Newtonsoft::Json;

	bool flag;
	std::vector<std::thread> threads;
	std::unordered_map<std::string, int> mapRooms;
	std::string stateTable[] = {
		"Hide", "Normal", "Minimized", "Maximized",
		"Not activated", "Show", "Minimized", "Minimized and Not activated",
		"Show and Not activated", "Restore"
	};

	void MarshalString(String^ s, std::string& os) {
		using namespace Runtime::InteropServices;
		const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}

	void sendMsg(HWND hWnd, std::string& msg) {
		SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)msg.c_str());
		Sleep(200);
		PostMessage(hWnd, WM_KEYDOWN, VK_RETURN, 0);
		Sleep(200);
		PostMessage(hWnd, WM_KEYUP, VK_RETURN, 0);
	}

	bool checkTime(std::string& time) {
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		
		std::ostringstream oss;
		oss << std::put_time(&tm, "%H:%M");
		
		return !oss.str().compare(time) && tm.tm_sec < 3;
	}

	void threadProc(std::string& t, std::string& m) {
		std::string time(t.c_str());
		std::string msg(m.c_str());
		
		while (!flag) {
			if (checkTime(time)) {
				for (const auto& key : mapRooms) {
					HWND hRoom = FindWindow("#32770", key.first.c_str());
					HWND hText = GetWindow(hRoom, GW_CHILD);

					if (!hRoom || !hText)
						continue;

					sendMsg(hText, msg);
				}

				Sleep(3000);
			}

			Sleep(200);
		}
	}

	/// <summary>
	/// Summary for Main
	/// </summary>
	public ref class Main : public System::Windows::Forms::Form
	{
	public:
		Main(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Main()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:
		ref class Config
		{
			public: String^ msg;
			public: array<String^>^ times;
		};

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;

	private: System::Windows::Forms::TextBox^ txtMsg;
	private: System::Windows::Forms::ListBox^ lstTime;
	private: System::Windows::Forms::ListView^ lvRoom;
	private: System::Windows::Forms::ColumnHeader^ columnHeader1;
	private: System::Windows::Forms::ColumnHeader^ columnHeader2;
	private: System::Windows::Forms::DateTimePicker^ timePicker;

	private: System::Windows::Forms::Button^ btnCtrl;
	private: System::Windows::Forms::Button^ btnRefresh;
	private: System::Windows::Forms::Button^ btnAddTime;
	private: System::Windows::Forms::Button^ btnDelTime;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->txtMsg = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->lvRoom = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->timePicker = (gcnew System::Windows::Forms::DateTimePicker());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->btnCtrl = (gcnew System::Windows::Forms::Button());
			this->btnRefresh = (gcnew System::Windows::Forms::Button());
			this->lstTime = (gcnew System::Windows::Forms::ListBox());
			this->btnAddTime = (gcnew System::Windows::Forms::Button());
			this->btnDelTime = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// txtMsg
			// 
			this->txtMsg->Location = System::Drawing::Point(10, 36);
			this->txtMsg->Multiline = true;
			this->txtMsg->Name = L"txtMsg";
			this->txtMsg->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtMsg->Size = System::Drawing::Size(332, 336);
			this->txtMsg->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(11, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(63, 19);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Message";
			// 
			// lvRoom
			// 
			this->lvRoom->CheckBoxes = true;
			this->lvRoom->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) { this->columnHeader1, this->columnHeader2 });
			this->lvRoom->FullRowSelect = true;
			this->lvRoom->HideSelection = false;
			this->lvRoom->Location = System::Drawing::Point(347, 36);
			this->lvRoom->Name = L"lvRoom";
			this->lvRoom->Size = System::Drawing::Size(355, 336);
			this->lvRoom->TabIndex = 2;
			this->lvRoom->UseCompatibleStateImageBehavior = false;
			this->lvRoom->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Title";
			this->columnHeader1->Width = 220;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"State";
			this->columnHeader2->Width = 100;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(347, 14);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(141, 19);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Detected chat rooms";
			// 
			// timePicker
			// 
			this->timePicker->CustomFormat = L"HH:mm";
			this->timePicker->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->timePicker->Location = System::Drawing::Point(10, 406);
			this->timePicker->Name = L"timePicker";
			this->timePicker->ShowUpDown = true;
			this->timePicker->Size = System::Drawing::Size(182, 25);
			this->timePicker->TabIndex = 4;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(11, 384);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(98, 19);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Time (HH:mm)";
			// 
			// btnCtrl
			// 
			this->btnCtrl->Location = System::Drawing::Point(597, 387);
			this->btnCtrl->Name = L"btnCtrl";
			this->btnCtrl->Size = System::Drawing::Size(105, 44);
			this->btnCtrl->TabIndex = 7;
			this->btnCtrl->Text = L"Start";
			this->btnCtrl->UseVisualStyleBackColor = true;
			this->btnCtrl->Click += gcnew System::EventHandler(this, &Main::btnCtrl_Click);
			// 
			// btnRefresh
			// 
			this->btnRefresh->Location = System::Drawing::Point(486, 387);
			this->btnRefresh->Name = L"btnRefresh";
			this->btnRefresh->Size = System::Drawing::Size(105, 44);
			this->btnRefresh->TabIndex = 8;
			this->btnRefresh->Text = L"&Refresh";
			this->btnRefresh->UseVisualStyleBackColor = true;
			this->btnRefresh->Click += gcnew System::EventHandler(this, &Main::btnRefresh_Click);
			// 
			// lstTime
			// 
			this->lstTime->FormattingEnabled = true;
			this->lstTime->ItemHeight = 17;
			this->lstTime->Location = System::Drawing::Point(10, 438);
			this->lstTime->Name = L"lstTime";
			this->lstTime->Size = System::Drawing::Size(332, 89);
			this->lstTime->TabIndex = 9;
			// 
			// btnAddTime
			// 
			this->btnAddTime->Location = System::Drawing::Point(198, 406);
			this->btnAddTime->Name = L"btnAddTime";
			this->btnAddTime->Size = System::Drawing::Size(69, 26);
			this->btnAddTime->TabIndex = 10;
			this->btnAddTime->Text = L"Add";
			this->btnAddTime->UseVisualStyleBackColor = true;
			this->btnAddTime->Click += gcnew System::EventHandler(this, &Main::btnAddTime_Click);
			// 
			// btnDelTime
			// 
			this->btnDelTime->Location = System::Drawing::Point(273, 406);
			this->btnDelTime->Name = L"btnDelTime";
			this->btnDelTime->Size = System::Drawing::Size(69, 26);
			this->btnDelTime->TabIndex = 11;
			this->btnDelTime->Text = L"Delete";
			this->btnDelTime->UseVisualStyleBackColor = true;
			this->btnDelTime->Click += gcnew System::EventHandler(this, &Main::btnDelTime_Click);
			// 
			// Main
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 17);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(712, 546);
			this->Controls->Add(this->btnDelTime);
			this->Controls->Add(this->btnAddTime);
			this->Controls->Add(this->lstTime);
			this->Controls->Add(this->btnRefresh);
			this->Controls->Add(this->btnCtrl);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->timePicker);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->lvRoom);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->txtMsg);
			this->Font = (gcnew System::Drawing::Font(L"Malgun Gothic", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"Main";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Megaphone";
			this->Load += gcnew System::EventHandler(this, &Main::Main_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		
	private:
		private: System::Void addRoom(String^ title, String^ state) {
			ListViewItem^ lvi = gcnew ListViewItem(title);
			lvi->Name = title;
			lvi->SubItems->Add(state);
			lvRoom->Items->Add(lvi);
		}

		private: System::Void refreshRoom() {
			std::string buffer(MAX_PATH, 0);
			for (HWND hWnd = FindWindow(NULL, NULL); hWnd; hWnd = GetWindow(hWnd, GW_HWNDNEXT)) {
				GetClassName(hWnd, &buffer[0], MAX_PATH);
				if (buffer.compare(0, 6, "#32770"))
					continue;
				
				DWORD pid = 0;
				GetWindowThreadProcessId(hWnd, &pid);

				HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
				DWORD bufSize = MAX_PATH;
				QueryFullProcessImageName(hProcess, 0, &buffer[0], &bufSize);
				CloseHandle(hProcess);

				if (buffer.rfind("KakaoTalk.exe") == std::string::npos)
					continue;
			
				WINDOWINFO wi = { 0 };
				GetWindowInfo(hWnd, &wi);

				if (wi.dwStyle & WS_POPUP)
					continue;

				GetWindowText(hWnd, &buffer[0], 1024);
				
				WINDOWPLACEMENT wndpl = { 0 };
				GetWindowPlacement(hWnd, &wndpl);

				String^ key = gcnew String(buffer.c_str());
				String^ state = gcnew String(stateTable[wndpl.showCmd].c_str());

				if (lvRoom->Items->ContainsKey(key)) {
					lvRoom->Items->Find(key, false)[0]->SubItems[1]->Text = state;
					continue;
				}
			
				addRoom(key, state);
			}
			
			for (int i = 0; i < lvRoom->Items->Count; i++) {
				MarshalString(lvRoom->Items[i]->Text, buffer);
				if (!FindWindow("#32770", buffer.c_str()))
					lvRoom->Items[i]->Remove();
			}
		}

		private: System::Void loadConfig() {
			if (!File::Exists("config.json"))
				return;

			String^ json = File::ReadAllText("config.json");
			Config^ config = JsonConvert::DeserializeObject<Config^>(json);

			txtMsg->Text = config->msg;
			txtMsg->Focus();
			txtMsg->SelectionStart = txtMsg->Text->Length;
			
			for (int i = 0; i < config->times->Length; i++)
				lstTime->Items->Add(config->times[i]);
		}

		private: System::Void setEnabled(bool enabled) {
			txtMsg->Enabled = enabled;
			timePicker->Enabled = enabled;
			btnAddTime->Enabled = enabled;
			btnDelTime->Enabled = enabled;
			lstTime->Enabled = enabled;
			lvRoom->Enabled = enabled;
			btnRefresh->Enabled = enabled;
		}

		private: System::Void Main_Load(System::Object^ sender, System::EventArgs^ e) {
			refreshRoom();
			loadConfig();
		}

		private: System::Void btnCtrl_Click(System::Object^ sender, System::EventArgs^ e) {
			if (btnCtrl->Text == "Stop") {
				flag = true;
				threads.clear();
				mapRooms.clear();

				setEnabled(true);
				btnCtrl->Text = "Start";
			}
			else {
				if (!txtMsg->Text->Length) {
					MessageBox::Show("Please write your message.", this->Text, MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return;
				}

				if (!lvRoom->Items->Count) {
					MessageBox::Show("Please open chat rooms.", this->Text, MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return;
				}

				if (!lstTime->Items->Count) {
					MessageBox::Show("Please add the time.", this->Text, MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return;
				}
				
				for (int i = 0; i < lvRoom->Items->Count; i++) {
					if (lvRoom->Items[i]->Checked) {
						std::string title;
						MarshalString(lvRoom->Items[i]->Text, title);
						mapRooms[title] = 1;
					}
				}

				if (!mapRooms.size()) {
					MessageBox::Show("Please check chat rooms.", this->Text, MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return;
				}

				std::string msg;
				MarshalString(txtMsg->Text, msg);

				Config^ config = gcnew Config();
				config->msg = txtMsg->Text;
				config->times = gcnew array<String^>(lstTime->Items->Count);

				flag = false;
				for (int i = 0; i < lstTime->Items->Count; i++) {
					std::string time;
					String^ item = lstTime->Items[i]->ToString();
					MarshalString(item, time);
					
					threads.emplace_back(threadProc, time, msg);
					threads.back().detach();

					config->times[i] = item;
				}

				File::WriteAllText("config.json", JsonConvert::SerializeObject(config));
				
				setEnabled(false);
				btnCtrl->Text = "Stop";
			}
		}

		private: System::Void btnRefresh_Click(System::Object^ sender, System::EventArgs^ e) {
			refreshRoom();
		}
		
		private: System::Void btnAddTime_Click(System::Object^ sender, System::EventArgs^ e) {
			if (lstTime->Items->Contains(timePicker->Text))
				MessageBox::Show("The selected time is already in the list.", this->Text, MessageBoxButtons::OK, MessageBoxIcon::Warning);
			else
				lstTime->Items->Add(timePicker->Text);
		}

		private: System::Void btnDelTime_Click(System::Object^ sender, System::EventArgs^ e) {
			if (!lstTime->SelectedItems->Count)
				return;

			lstTime->Items->RemoveAt(lstTime->SelectedIndex);
		}


	};
}