#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;



public ref class AccountChangedEventArgs : EventArgs // �¼�������
		{
		public:
			AccountChangedEventArgs( String^ nodeId, String^ nodeName )
			{
				this->nodeId = nodeId;
				this->nodeName = nodeName;
			}
			String^ nodeId;
			String^ nodeName;
		};