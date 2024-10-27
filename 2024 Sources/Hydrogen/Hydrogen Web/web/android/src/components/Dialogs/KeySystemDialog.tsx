import {
  ActionIcon,
  Anchor,
  Button,
  Group,
  List,
  Loader,
  Modal,
  Stack,
  Text,
  TextInput,
} from "@mantine/core";
import { useDisclosure } from "@mantine/hooks";
import { IconDroplet, IconKey, IconLogin } from "@tabler/icons";
import { CSSProperties, FC, useEffect } from "react";
import { InfoDialog } from "./InfoDialog";

type Props = {
  transitionStyles: CSSProperties;
  opened?: boolean;
};

export const KeySystemDialog: FC<Props> = ({
  transitionStyles,
  opened: defaultOpened = false,
}) => {
  const [opened, handlers] = useDisclosure(defaultOpened);

  useEffect(() => {
    if (defaultOpened) handlers.open();
    else handlers.close();
  }, [defaultOpened]);

  return (
    <>
      <ActionIcon
        radius="xl"
        variant="outline"
        size={64}
        style={transitionStyles}
        onClick={handlers.open}
      >
        <IconDroplet size={40} />
      </ActionIcon>

      <Modal
        padding="xl"
        centered={true}
        trapFocus={false}
        overlayBlur={3.5}
        transition="fade"
        title="Login to your account"
        onClose={handlers.close}
        opened={opened}
      >
        {/* <Button.Group>
          <Button
            radius="sm"
            size="lg"
            variant="default"
            leftIcon={<IconKey />}
            sx={{
              flexGrow: 1,
              fontSize: 17,
            }}
          >
            Get Key
          </Button>
          <Button
            radius="sm"
            size="lg"
            variant="default"
            leftIcon={<IconLogin />}
            sx={{
              flexGrow: 1,
              fontSize: 17,
            }}
          >
            Login
          </Button>
        </Button.Group> */}
        <Stack>
          <TextInput
            description={<InfoDialog />}
            placeholder="License Key"
            sx={{
              transitionDuration: "150ms",
            }}
          />
          <Button
            color="blue.4"
            sx={{
              transitionDuration: "150ms",
            }}
          >
            Login
          </Button>
        </Stack>
      </Modal>
    </>
  );
};
