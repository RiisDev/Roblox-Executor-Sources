# Only For UWP 

|Education Purpose Only.|
|-------------------------------------------------|
This project was created only for good purposes and personal use.
Please be aware that your action have consequences.
Becareful Using this tool, i am warning you to use this tool for Education Purpose Only 👀.


# how to use?

Our discord server 'https://discord.gg/YQNAcbgenG'

# Dumper  

if you don't know every roblox update we need to update address / offest. so now you can use Dumper for it 

1: open the dumper and make sure Roblox (UWP) Is open 

2: Copy the file

3: goto Addresses.hpp and replace all 

# Inject

Inject cmd comment inj.exe YT.dll Windows10Universal.exe

or c# code for inject 

    Process.Start("inj.exe", "YT.dll Windows10Universal.exe")

Release x86

![image](https://github.com/YTGuest/Roblox-UWP-API/assets/141551264/442f002d-7c8a-43d5-97ed-02675baf3f27)


Execute Function C#: 

    private void Execute_Click(object sender, EventArgs e)
    {
        string username = Environment.UserName;
        string directoryPath = $@"C:\Users\{username}\AppData\Local\Packages\ROBLOXCORPORATION.ROBLOX_55nm5eh3cm0pr\AC";
        string filename = "Execute.lua";
        string filePath = Path.Combine(directoryPath, filename);
    
        string script_t = "print("test")";
    
        try
        {
            using (StreamWriter writer = File.CreateText(filePath))
            {
                writer.Write(script_t);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine("Error: " + ex.Message);
        }       
    }


# credits

`scheduler` http://github.com/NezyDev/

`compiler` https://github.com/roblox/luau/

`project` Nezy#9092

`Dumper offset` U_M9
