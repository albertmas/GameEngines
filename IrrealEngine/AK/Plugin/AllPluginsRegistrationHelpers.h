/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the 
"Apache License"); you may not use this file except in compliance with the 
Apache License. You may obtain a copy of the Apache License at 
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Version: v2018.1.3  Build: 6784
  Copyright (c) 2006-2018 Audiokinetic Inc.
*******************************************************************************/

/// \file 
///This file is used only to provide backward compatibility with the previous plugin system.
///Please read the SDK documentation regarding effect plugin registration.
///\sa quickstart_sample_integration_plugins
#include <AK/Plugin/AllPluginsFactories.h>

namespace AK
{
	namespace SoundEngine
	{
		/// This function is deprecated.  All registration is automatic now.  Kept for backward compatibility.
		static AKRESULT RegisterAllPlugins()
		{
			return AK_Success;
		}
	}
}
