using System;
using System.CodeDom.Compiler;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.Resources;
using System.Runtime.CompilerServices;

namespace ClubDark.Properties
{
	// Token: 0x02000004 RID: 4
	[GeneratedCode("System.Resources.Tools.StronglyTypedResourceBuilder", "15.0.0.0")]
	[DebuggerNonUserCode]
	[CompilerGenerated]
	internal class Resources
	{
		// Token: 0x0600002B RID: 43 RVA: 0x00005B22 File Offset: 0x00003D22
		internal Resources()
		{
		}

		// Token: 0x17000001 RID: 1
		// (get) Token: 0x0600002C RID: 44 RVA: 0x00005B2C File Offset: 0x00003D2C
		[EditorBrowsable(EditorBrowsableState.Advanced)]
		internal static ResourceManager ResourceManager
		{
			get
			{
				bool flag = Resources.resourceMan == null;
				if (flag)
				{
					ResourceManager resourceManager = new ResourceManager("ClubDark.Properties.Resources", typeof(Resources).Assembly);
					Resources.resourceMan = resourceManager;
				}
				return Resources.resourceMan;
			}
		}

		// Token: 0x17000002 RID: 2
		// (get) Token: 0x0600002D RID: 45 RVA: 0x00005B74 File Offset: 0x00003D74
		// (set) Token: 0x0600002E RID: 46 RVA: 0x00005B8B File Offset: 0x00003D8B
		[EditorBrowsable(EditorBrowsableState.Advanced)]
		internal static CultureInfo Culture
		{
			get
			{
				return Resources.resourceCulture;
			}
			set
			{
				Resources.resourceCulture = value;
			}
		}

		// Token: 0x17000003 RID: 3
		// (get) Token: 0x0600002F RID: 47 RVA: 0x00005B94 File Offset: 0x00003D94
		internal static Bitmap inject
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("inject", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		// Token: 0x17000004 RID: 4
		// (get) Token: 0x06000030 RID: 48 RVA: 0x00005BC4 File Offset: 0x00003DC4
		internal static Bitmap play_button_flat_fm100_salmon
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("play-button-flat-fm100-salmon", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		// Token: 0x17000005 RID: 5
		// (get) Token: 0x06000031 RID: 49 RVA: 0x00005BF4 File Offset: 0x00003DF4
		internal static Bitmap Render
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("Render", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		// Token: 0x0400003F RID: 63
		private static ResourceManager resourceMan;

		// Token: 0x04000040 RID: 64
		private static CultureInfo resourceCulture;
	}
}
