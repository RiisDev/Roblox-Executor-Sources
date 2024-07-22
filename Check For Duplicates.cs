List<string> yearDirectories = Directory.GetDirectories(@"G:\Sources\Roblox-Executor-Sources", "*").ToList();
yearDirectories.RemoveAt(0);
IEnumerable<string> exploitFolders = yearDirectories.SelectMany(yearDirectory => Directory.GetDirectories(yearDirectory, "*"));

Dictionary<string, string[]> exploitContents = [];

foreach (string exploitFolder in exploitFolders) AddStuff(exploitFolder);

foreach (KeyValuePair<string, string[]> exploit in exploitContents)
{
    IEnumerable<KeyValuePair<string, string[]>> dataFound = exploitContents.Where((data) => data.Key != exploit.Key && data.Value.SequenceEqual(exploit.Value));
    IEnumerable<KeyValuePair<string, string[]>> keyValuePairs = dataFound as KeyValuePair<string, string[]>[] ?? dataFound.ToArray();
    if (keyValuePairs.Any())
    {
        if (keyValuePairs.First().Key == "AutoPCH") continue;
        Console.WriteLine($"Found duplicate data: {exploit.Key} {exploit.Value.Length} and {keyValuePairs.First().Key} {keyValuePairs.First().Value.Length}");
    }
}


return;
IEnumerable<string> FilterFiles(string path, string[] exts)
{
    return
        Directory
            .EnumerateFiles(path, "*.*")
            .Where(file => !exts.Any(x => file.EndsWith(x, StringComparison.OrdinalIgnoreCase)));
}

void AddStuff(string exploitFolder)
{
    bool doBreak = false;
    DirectoryInfo dirInfo = new(exploitFolder);
    List<string> files = FilterFiles(exploitFolder, ["md", "sln", "vcproj", "vcxproj", "filters", "user", "exe", "rar", "zip"]).ToList();
    string[] internalDirectories = Directory.GetDirectories(exploitFolder);
    
    if (exploitFolder.Contains("monaco", StringComparison.OrdinalIgnoreCase))

    if (internalDirectories.Length > 0) 
        foreach (string internalDirectory in internalDirectories) 
            AddStuff(internalDirectory);
    
    switch (files.Count)
    {
        case 0 when internalDirectories.Length == 0:
            Console.WriteLine($"Empty Folder {exploitFolder}");
            doBreak = true;
            break;
        case 1 when new FileInfo(files[0]).Length == 0:
            Console.WriteLine($"Empty Folder (Dead File) {exploitFolder}");
            doBreak = true;
            break;
    }


    int counter = 1;
    string exploitName = dirInfo.Name;
    string originalName = exploitName;
    while (exploitContents.ContainsKey(exploitName))
    {
        exploitName = $"{originalName} ({counter})";
        counter++;
    }

    exploitContents.Add(exploitFolder, Directory.GetFiles(exploitFolder));
}