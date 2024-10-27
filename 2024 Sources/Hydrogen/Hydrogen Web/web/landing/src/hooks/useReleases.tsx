import { ReleaseObject } from "@hydrogen-web/interfaces";
import { useOs } from "@mantine/hooks";
import {
  IconDownload,
  IconBrandApple,
  IconBrandWindows,
  IconBrandAndroid,
} from "@tabler/icons";
import { useEffect, useState } from "react";
import { useApi } from "./useApi";

export const useReleases = () => {
  const { fetch } = useApi();
  const sysOs = useOs();

  const [releases, setReleases] = useState<ReleaseObject[]>();
  const [os, setOs] = useState("UNDETERMINED");

  useEffect(() => {
    fetch<ReleaseObject[]>({
      method: "GET",
      url: "/releases",
    }).then((response) => {
      setReleases(response.data);
    });
  }, []);

  useEffect(() => {
    if (!releases) return;

    if (
      sysOs &&
      releases
        .map((release) => release.platform)
        .includes(sysOs.toUpperCase() as any)
    )
      setOs(sysOs.toUpperCase());
    else setOs("ANDROID");
  }, [sysOs, releases]);

  return {
    releases,
    os,
    formattedOs: {
      UNDETERMINED: "Free",
      MACOS: "macOS",
      IOS: "iOS",
      WINDOWS: "Windows",
      ANDROID: "Android",
      LINUX: "Free",
    }[os],
    osIcon: {
      UNDETERMINED: <IconDownload />,
      MACOS: <IconBrandApple />,
      IOS: <IconBrandApple />,
      WINDOWS: <IconBrandWindows />,
      ANDROID: <IconBrandAndroid />,
      LINUX: null,
    }[os],
    downloadText: {
      UNDETERMINED: "Now Available for Free",
      MACOS: "Now Available on macOS",
      IOS: "Now Available on iOS",
      WINDOWS: "Now Available on Windows",
      ANDROID: "Now Available on Android",
      LINUX: "Now Available for Free",
    }[os],
  };
};
