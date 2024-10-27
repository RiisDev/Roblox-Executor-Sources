import { Anchor, List, Modal, Stack, Text } from "@mantine/core";
import { useDisclosure } from "@mantine/hooks";
import { FC } from "react";

export const InfoDialog: FC = () => {
  const [opened, handlers] = useDisclosure(false);

  return (
    <>
      <Anchor
        color="gray.3"
        size={14}
        sx={{
          maxInlineSize: 315,
        }}
        onClick={handlers.open}
      >
        Learn more about pricing and our Key System
      </Anchor>

      <Modal
        size="lg"
        padding="xl"
        centered={true}
        trapFocus={false}
        overlayBlur={3.5}
        overflow="inside"
        transition="fade"
        title="About Our Key System"
        onClose={handlers.close}
        opened={opened}
      >
        <Stack>
          <Text size="sm" color="dimmed">
            In order to keep Hydrogen free for everyone to use, we have adopted
            ad-based key system. You can avoid this system through a one-time
            lifetime purchase of{" "}
            <Anchor component="span" color="blue.2">
              Hydrogen AdLess
            </Anchor>
            .
          </Text>

          <Stack spacing="md">
            <Text size="md">How does the key system work?</Text>
            <Text size="sm" color="dimmed">
              While we understand many people are against a key system, it is
              not our intention to block our service through many ad barriers.
              <br />
              <br />
              Keep in mind that the process must be started through the loader,{" "}
              <strong>do NOT bookmark this page</strong>. The process for
              getting a key can be seen below:
            </Text>

            <List size="sm" type="ordered" spacing={5}>
              <List.Item>Open the Hydrogen Loader</List.Item>
              <List.Item>
                Click &quot;Get Key,&quot; a new tab will be opened to our
                gateway site
              </List.Item>
              <List.Item>
                Complete the captcha and click &quot;Get Key&quot;
              </List.Item>
              <List.Item>
                <Text>
                  There will be <strong>3</strong> ad checkpoints, complete each
                  one
                </Text>
              </List.Item>
              <List.Item>
                You will be brought back to the gateway and be issued a key
              </List.Item>
            </List>

            <Text color="dimmed" size="sm">
              Please note that keys automatically expire 24 hours after
              generation, after 24 hours, you will need to repeat the process
              again.
            </Text>
          </Stack>

          <Stack spacing="md">
            <Text size="md">What is Hydrogen AdLess?</Text>
            <Text size="sm" color="dimmed">
              <Anchor component="span" color="blue.2">
                Hydrogen AdLess
              </Anchor>{" "}
              is a true lifetime purchase that will allow you to bypass our the
              key system and obtain a permanent license for Hydrogen. Our
              premium plan includes other perks like having early insider info
              on new updates, premium support, and more.
            </Text>
          </Stack>
        </Stack>
      </Modal>
    </>
  );
};
