import {
  ActionIcon,
  Anchor,
  Box,
  Button,
  Loader,
  Stack,
  Text,
  Transition,
} from "@mantine/core";
import { Logo } from "components/Logo";
import { NextPage } from "next";
import { IconDroplet } from "@tabler/icons";
import { InfoDialog } from "components/Dialogs/InfoDialog";
import { useEffect, useState } from "react";
import { KeySystemDialog } from "components/Dialogs/KeySystemDialog";
import { MainWindowDialog } from "components/Dialogs/MainWindowDialog";

const Page: NextPage = () => {
  const [loading, setLoading] = useState(true);
  const [loaded, setLoaded] = useState(false);
  const [defaultOpened, setDefaultOpened] = useState(false);

  useEffect(() => {
    setTimeout(() => {
      setLoading(false);
      setDefaultOpened(true);
      setTimeout(() => {
        setLoaded(true);
      }, 500);
    }, 1000);
  }, []);

  return (
    <Stack
      align="center"
      justify="center"
      sx={{
        flexGrow: 1,
      }}
    >
      <Transition mounted={loaded} transition="fade" duration={200}>
        {(styles) => (
          <MainWindowDialog opened={defaultOpened} transitionStyles={styles} />
        )}
      </Transition>

      <Transition mounted={loading} transition="fade" duration={150}>
        {(styles) => (
          <Stack align="center" style={styles}>
            <Text color="gray.3" size={14}>
              Please wait while we get things ready...
            </Text>
            <Loader variant="dots" color="gray" />
          </Stack>
        )}
      </Transition>
    </Stack>
  );
};

export default Page;
