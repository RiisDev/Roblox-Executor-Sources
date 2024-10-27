import {
  ActionIcon,
  AppShell,
  Avatar,
  Box,
  Button,
  CloseButton,
  Group,
  Header,
  Modal,
  Navbar,
  Stack,
  Text,
  ThemeIcon,
  UnstyledButton,
  useMantineTheme,
} from "@mantine/core";
import { useDisclosure } from "@mantine/hooks";
import { openConfirmModal } from "@mantine/modals";
import Editor, { useMonaco } from "@monaco-editor/react";
import {
  IconChevronLeft,
  IconChevronRight,
  IconClock,
  IconCode,
  IconDroplet,
  IconFileExport,
  IconFolder,
  IconHome,
  IconPlayerPlay,
  IconScript,
  IconSettings,
  IconTrash,
} from "@tabler/icons";
import { Logo } from "components/Logo";
import { CSSProperties, FC, useEffect } from "react";
import { iyTest } from "./file";

type Props = {
  transitionStyles: CSSProperties;
  opened?: boolean;
};

export const MainWindowDialog: FC<Props> = ({
  transitionStyles,
  opened: defaultOpened = false,
}) => {
  const theme = useMantineTheme();
  const monaco = useMonaco();
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
        size="xl"
        radius="sm"
        centered={true}
        trapFocus={false}
        overlayBlur={3.5}
        transition="slide-up"
        onClose={handlers.close}
        opened={opened}
        overflow="inside"
        withCloseButton={false}
        padding={theme.spacing.xs + 5}
        closeOnEscape={false}
        closeOnClickOutside={false}
        styles={{
          modal: {
            paddingTop: "0px !important",
          },
        }}
      >
        <AppShell
          fixed={false}
          navbar={
            <Navbar height={350} pt="xs" pr="xs" width={{ base: 190 }}>
              <Navbar.Section grow mt={5} mb="xs">
                <Stack
                  spacing={5}
                  sx={{
                    height: "100%",
                  }}
                >
                  {[
                    {
                      icon: <IconHome size={16} />,
                      color: "gray",
                      label: "Home",
                    },
                    {
                      icon: <IconCode size={16} />,
                      color: "gray",
                      label: "Editor",
                    },
                    {
                      icon: <IconScript size={16} />,
                      color: "gray",
                      label: "Scripts",
                    },
                    {
                      icon: <IconSettings size={16} />,
                      color: "gray",
                      label: "Settings",
                    },
                  ].map(({ icon, color, label }, i) => (
                    <UnstyledButton
                      key={i}
                      sx={(theme) => ({
                        marginTop: i === 3 ? "auto" : "initial",
                        display: "block",
                        width: "100%",
                        paddingLeft: 10,
                        paddingRight: 10,
                        paddingTop: 8,
                        paddingBottom: 8,
                        borderRadius: theme.radius.sm,
                        color:
                          theme.colorScheme === "dark"
                            ? theme.colors.dark[0]
                            : theme.black,
                        backgroundColor:
                          i === 1
                            ? theme.colorScheme === "dark"
                              ? theme.colors.dark[5]
                              : theme.colors.gray[0]
                            : "initial",
                        "&:hover": {
                          backgroundColor:
                            theme.colorScheme === "dark"
                              ? theme.colors.dark[6]
                              : theme.colors.gray[0],
                        },
                        transitionDuration: "50ms",
                      })}
                    >
                      <Group>
                        <ThemeIcon color={color} variant="light">
                          {icon}
                        </ThemeIcon>

                        <Text size="sm">{label}</Text>
                      </Group>
                    </UnstyledButton>
                  ))}
                </Stack>
              </Navbar.Section>
              <Navbar.Section>
                <Box
                  sx={{
                    paddingTop: theme.spacing.sm,
                    borderTop: `1px solid ${
                      theme.colorScheme === "dark"
                        ? theme.colors.dark[4]
                        : theme.colors.gray[2]
                    }`,
                  }}
                >
                  <UnstyledButton
                    sx={{
                      display: "block",
                      width: "100%",
                      padding: theme.spacing.xs,
                      borderRadius: theme.radius.sm,
                      color:
                        theme.colorScheme === "dark"
                          ? theme.colors.dark[0]
                          : theme.black,

                      "&:hover": {
                        backgroundColor:
                          theme.colorScheme === "dark"
                            ? theme.colors.dark[6]
                            : theme.colors.gray[0],
                      },
                    }}
                  >
                    <Group>
                      <Box sx={{ flex: 1 }}>
                        <Text size="sm" weight={500}>
                          aspect
                        </Text>
                        <Text color="dimmed" size="xs">
                          View Profile
                        </Text>
                      </Box>

                      {theme.dir === "ltr" ? (
                        <IconChevronRight size={18} />
                      ) : (
                        <IconChevronLeft size={18} />
                      )}
                    </Group>
                  </UnstyledButton>
                </Box>
              </Navbar.Section>
            </Navbar>
          }
          header={
            <Header
              height={50}
              sx={{
                display: "flex",
                alignItems: "center",
                paddingTop: 33,
                paddingBottom: 33,
              }}
            >
              <Group
                mt={3}
                position="apart"
                sx={{
                  width: "100%",
                }}
              >
                <Group spacing={5} align="flex-end">
                  <Logo />
                  <Text
                    sx={{
                      userSelect: "none",
                      fontSize: 12,
                    }}
                  >
                    v1.1.0-alpha
                  </Text>
                </Group>
                <Group spacing={17} align="center">
                  {/* <Select
                    size="xs"
                    data={[
                      {
                        label: "English",
                        value: "ENGLISH",
                      },
                      {
                        label: "Español",
                        value: "SPANISH",
                      },
                      {
                        label: "Português (Brasil)",
                        value: "PORTUGUESE",
                      },
                      {
                        label: "Italiano",
                        value: "ITALIAN",
                      },
                      {
                        label: "Français",
                        value: "FRENCH",
                      },
                      {
                        label: "Deutsch",
                        value: "GERMAN",
                      },
                      {
                        label: "عربي",
                        value: "ARABIC",
                      },
                      {
                        label: "Română",
                        value: "ROMANIAN",
                      },
                      {
                        label: "ไทย",
                        value: "THAI",
                      },
                    ]}
                    icon={<IconLanguage size={16} />}
                    defaultValue="English"
                    variant="unstyled"
                    sx={{
                      maxWidth: 150,
                    }}
                  /> */}
                  <Group
                    spacing={9}
                    align="center"
                    sx={{
                      userSelect: "none",
                    }}
                  >
                    <IconClock size={16} />
                    <Text size="sm">
                      Time Remaining:{" "}
                      <Text weight={600} component="span">
                        23 Hours
                      </Text>
                    </Text>
                  </Group>

                  <CloseButton
                    size={24}
                    onClick={() => {
                      handlers.close();
                    }}
                  />
                </Group>
              </Group>
            </Header>
          }
          styles={() => ({
            main: {
              padding: 0,
              display: "flex",
              flexDirection: "column",
              justifyContent: "space-between",
              overflow: "hidden",
            },
          })}
        >
          <Editor
            height="88.7%"
            theme="vs-dark"
            language="lua"
            defaultValue={iyTest}
          />
          <Button.Group
            sx={{
              justifyContent: "center",
            }}
          >
            <Button
              sx={{
                flexGrow: 1,
              }}
              radius={0}
              size="sm"
              variant="default"
              color="gray"
              leftIcon={<IconPlayerPlay size={16} color="gray" />}
            >
              Execute
            </Button>
            <Button
              sx={{
                flexGrow: 1,
              }}
              radius={"sm"}
              size="sm"
              variant="default"
              color="gray"
              leftIcon={<IconTrash size={16} color="gray" />}
              onClick={() => {
                openConfirmModal({
                  zIndex: 99999,
                  title: "Clear Editor",
                  centered: true,
                  children: (
                    <Text size="sm">
                      Are you sure you want to clear the editor? This action is
                      irreversible.
                    </Text>
                  ),
                  labels: {
                    confirm: "Clear Editor",
                    cancel: "Cancel",
                  },
                  confirmProps: { color: "red" },
                  onConfirm: () => {
                    monaco!.editor.getModels()[0].setValue("");
                  },
                });
              }}
            >
              Clear Editor
            </Button>
            <Button
              sx={{
                flexGrow: 1,
              }}
              radius={"sm"}
              size="sm"
              variant="default"
              color="gray"
              leftIcon={<IconFolder size={16} color="gray" />}
            >
              Open File
            </Button>
            <Button
              sx={{
                flexGrow: 1,
              }}
              radius={"sm"}
              size="sm"
              variant="default"
              color="gray"
              leftIcon={<IconFileExport size={16} color="gray" />}
            >
              Save File
            </Button>
          </Button.Group>
        </AppShell>
      </Modal>
    </>
  );
};
